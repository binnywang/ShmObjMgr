#include <iostream>
#include "shm_obj_mgr.h"
#include "obj_creator_mgr.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "shm_obj_test.h"

size_t InitShmPool() {

	/// register obj creator;
	ObjectCreatorMgr::Instance().RegisterObjectCreator<ClassA>();
	ObjectCreatorMgr::Instance().RegisterObjectCreator<ClassB>();

	GroupId group_id;
	ShmObjMgr::Instance().NewClassGroup(group_id);

	ShmObjMgr::Instance().AddClassToGroup<ClassA>(group_id, 10);
	ShmObjMgr::Instance().AddClassToGroup<ClassB>(group_id, 20);
	return ShmObjMgr::Instance().TotalSize();
}

void* GetShm(char* path_name, size_t max_size, bool & is_fresh) {
	if (path_name == NULL) {
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
	void* addr = GetShm("./shm_path_file", total_size, fresh);
	if (addr == (void*)-1) {
		std::cout << "Get Shm Error:" << errno << strerror(errno) << std::endl;
		return -1;
	}
	
	int ret = ShmObjMgr::Instance().Init(addr, total_size, fresh);
	if (ret != 0) {
		std::cout << "ShmObjMgr Init Error : " << ret << std::endl;
		return -1;
	}

	std::cout <<"shm address" << std::hex << addr << std::endl;
	
	if (fresh) {
		std::cout << "shm is fresh:" << std::endl; 

		/// 创建对象
		ShmObjPtr<ClassA> a_pointer = ShmObjMgr::Instance().CreateObject<ClassA>();
		if (! a_pointer) {
			std::cout << "Create ClassA Error: " << ret << std::endl;
			return -1;
		}

		/// 派生类指针可以隐士转换为基类
		ShmObjPtr<Base> base_pointer = a_pointer;

		/// 创建B对象
		ShmObjPtr<ClassB> b_pointer = ShmObjMgr::Instance().CreateObject<ClassB>();
		if (! b_pointer) {
			std::cout << "Create ClassB Error: " << ret << std::endl;	
			return -1;
		}


		std::cout << std::dec;

		/// 基类指针指向派生类
		base_pointer->Update();

		a_pointer->SetMember(111);
		a_pointer->Update();

		b_pointer->SetClassAObjPtr(a_pointer);
		b_pointer->SetMember(222);
		b_pointer->Update();
		
		std::cout <<"output pointer:" << std::endl;
		std::cout << "a_pointer.obj_id" << std::dec << a_pointer.obj_id().id << std::endl;
		std::cout << "b_pointer.obj_id" << std::dec << b_pointer.obj_id().id << std::endl;
		
		int a = 1/0;

		ShmObjMgr::Instance().FreeObject(a_pointer);
		ShmObjMgr::Instance().FreeObject(b_pointer);
		
	} else {

		std::cout << "shm is recovery: "<< std::endl;
		std::cout << "input a_pointer.obj_id" << std::endl;
		uint64_t id_a;
		std::cin >> id_a;
		ObjId obj_id_a;
		obj_id_a.id = id_a;
		ShmObjPtr<ClassA> a_pointer;
		a_pointer.set_obj_id(obj_id_a);
		
		std::cout << "input b_pointer.obj_id" << std::endl;

		uint64_t id_b;
		std::cin >> id_b;
		ObjId obj_id_b;
		obj_id_b.id = id_b;
		ShmObjPtr<ClassB> b_pointer;
		b_pointer.set_obj_id(obj_id_b);

		if(a_pointer) {
			a_pointer->Update();
		}

		if(b_pointer) {
			b_pointer->Update();
		}

		/// 释放对象
		ShmObjMgr::Instance().FreeObject(a_pointer);
		ShmObjMgr::Instance().FreeObject(b_pointer);
	}
	
	return 0;
}
