#include <iostream>
#include "shm_obj_mgr.h"
#include "obj_creator_mgr.h"
#include "shm_pointer.h"
#include "weak_shm_pointer.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "shm_obj_test.h"

size_t InitShmPool() {
	GroupId group_id;
	ShmObjMgr::Instance().NewClassGroup(group_id);

	ShmObjMgr::Instance().AddClassToGroup<ClassA>(group_id, 10);
	ShmObjMgr::Instance().AddClassToGroup<ClassB>(group_id, 20);
	return ShmObjMgr::Instance().TotalSize();
}

void* GetShm(const char* path_name, size_t max_size, bool & is_fresh) {
	if (path_name == NULL) {
		return NULL;
	}

	int fd = open(path_name, O_CREAT | O_RDONLY, 0644);
	if (fd < 0) {
		std::cout << "create error " << path_name << strerror(errno) << std::endl;
		return NULL;
	}

	struct stat buf;
	int ret = stat(path_name, &buf);
	if (ret != 0) {
		std::cout << "stat error" << errno << strerror(errno) << std::endl;
		return NULL;
	}
	key_t key = buf.st_ino;
	
	std::cout << "shmkey = " << std::hex << key << std::endl; 
	int shmid = shmget(key, max_size, 0666 & (~IPC_CREAT));
	if (shmid < 0) {
		shmid = shmget(key, max_size, 0666 | IPC_CREAT);
		if (shmid < 0) {
			std::cout << "create shm error" << errno << strerror(errno) << std::endl;
			return NULL;
		} else {
			is_fresh = true;
		}
	} else {
		is_fresh = false;
		std::cout << "is not fresh" << std::endl;
	}
	
	std::cout << "shmid = " << std::hex << shmid << std::endl;
	return shmat(shmid, NULL, 0);
}

template <typename Y, typename T> inline void static_cast_test() {
	T *p = static_cast<Y*>(0);
	(void)p;
}

int main(int argc, char* argv[]) {

	size_t total_size = InitShmPool();
	bool fresh = true;
	void* addr = GetShm("./shm_pointer_v2_file", total_size, fresh);
	if (addr == (void*)-1) {
		std::cout << "Get Shm Error:" << errno << strerror(errno) << std::endl;
		return -1;
	}
	
	int ret = ShmObjMgr::Instance().Init(addr, total_size, fresh);
	if (ret != 0) {
		std::cout << "ShmObjMgr Init Error : " << ret << std::endl;
		return -1;
	}

	std::cout <<"mem address = " << std::hex << addr << std::endl;
	
	if (fresh) {
		std::cout << "fresh info:" << std::endl; 
		ShmObjPtr<ClassA> ptr_obj_a = ShmObjMgr::Instance().CreateObject<ClassA>();
		ShmPointer<ClassA> a_pointer = ShmPointer<ClassA>(ptr_obj_a);
		if (! a_pointer) {
			std::cout << "Create ClassA Error: " << ret << std::endl;
			return -1;
		}

		ShmObjPtr<ClassB> ptr_obj_b = ShmObjMgr::Instance().CreateObject<ClassB>();
		ShmPointer<ClassB> b_pointer = ShmPointer<ClassB>(ptr_obj_b);

		if (! b_pointer) {
			std::cout << "Create ClassB Error: " << ret << std::endl;	
			return -1;
		}


		std::cout << std::dec;
		
		/// 测试基类指针调用派生类
		/// ShmPointer<Base> base_pointer_a = a_pointer;
		/// base_pointer_a->Update();

		/// 测试WeakShmPointer析构函数没有被调用的情况
		WeakShmPointer<ClassB> weak_pointer_b = WeakShmPointer<ClassB>(b_pointer);

		a_pointer->SetWeakShmPointerOfClassB(WeakShmPointer<ClassB>(b_pointer));
		a_pointer->SetMember(111);
		a_pointer->Update();

		b_pointer->SetShmPointerOfClassA(a_pointer);
		b_pointer->SetMember(222);


		
		b_pointer->Update();
		
		std::cout <<"output pointer" << std::endl;
		/// save pointer to file 
		std::cout << "a_pointer.obj_id = " << std::dec << ptr_obj_a.obj_id().id << std::endl;
		
		std::cout << "b_pointer.obj_id = " << std::dec << ptr_obj_b.obj_id().id << std::endl;


		int a = 1/0;
		
	} else {

		std::cout << "recovery info: "<< std::endl;

		std::cout << "input a_pointer.obj_id" << std::endl;
		uint64_t id_a;
		std::cin >> id_a;
		ObjId obj_id_a;
		obj_id_a.id = id_a;
		ShmObjPtr<ClassA> obj_ptr_a;
		obj_ptr_a.set_obj_id(obj_id_a);

		ShmPointer<ClassA> a_pointer;
		a_pointer.SetObjPtr(obj_ptr_a);
		
		std::cout << "input b_pointer.obj_id" << std::endl;

		uint64_t id_b;
		std::cin >> id_b;
		ObjId obj_id_b;
		obj_id_b.id = id_b;
		ShmObjPtr<ClassB> obj_ptr_b;
		obj_ptr_b.set_obj_id(obj_id_b);


		ShmPointer<ClassB> b_pointer;
		b_pointer.SetObjPtr(obj_ptr_b);
		
		if (a_pointer) {
			a_pointer->Update();
		}

		if (b_pointer) {
			b_pointer->Update();
		}
	}
	
	return 0;
}
