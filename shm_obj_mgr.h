#ifndef SHM_OBJ_MGR_H_
#define SHM_OBJ_MGR_H_

#include <new>
#include "singleton.h"
#include "shm_pool.h"

using namespace std;
class IObjectCreator {
public:	
	virtual void* ReplacementNew(void* addr) = 0;	
	virtual void* New() = 0;
};

template <class T>
class ObjectCreator : public IObjectCreator<ObjectCreator>, Singleton<ObjectCreator>{
public:
	DeclareSingleton(ObjectCreator);
	
	virtual void* New() {
		return new T();
	}
	
	virtual void* ReplacementNew(void * addr) {
		return new(T)addr;	
	}
};

enum ClassTypeEnum {	
	ClassA,	
	ClassB,	
	Count,
}

#define DeclareTypeName(ClassName)	static const ClassTypeEnum TYPE;
#define ImplmentTypeName(ClassName) const ClassTypeEnum ClassName::TYPE = ClassTypeEnum::ClassName;


class ObjectCreatorMgr : public Singleton<ObjectCreatorMgr> {
public:
	DeclareSingleton(ObjectCreatorMgr);
protected:	
	ObjectCreatorMgr() {		
	/*RegisterDynamicCreator<ObjectA>();		
	  RegisterDynamicCreator<ObjectB>();		
	  RegisterDynamicCreator<ObjectC>();*/
	}
	
	template <typename T>	
	T* CreateObject(void* addr)	{
		IObjectCreator* creator = obj_creator[T::TYPE];
		void* obj = creator->ReplacementNew(addr);
		return dynamic_cast<T>(obj);
	}		
	template <typename T> 
	int RegisterObjectCreator() {			
		obj_creator[T::typeId] = ObjectCreator<T>::Singleton();   
	}		

	IObjectCreator* obj_creator[ClassTypeEnum.Count];	
};



/// 共享内存指针
template <class T>
class ShmPointer {
public:
	friend class ShmObjMgr;
	void Recovery() {
		ShmObjMgr::Singleton().Recovery<T>(*this);	
	}
private: 
	T* obj_ptr; 
	ObjID obj_id;
};


class ShmObjMgr : public Singleton<ShmObjMgr>{
public:		
	DeclareSingleton(ShmObjMgr);

	void SetObjCreatorMgr(ObjectCreatorMgr* obj_creator_mgr) {
		obj_creator_mgr_ = obj_creator_mgr;
	}

	int NewClassGroup(GRPUPID& group_id) {
		return shm_pool_.NewClassGroup(group_id);
	}
	
	template<typename T>
	int AddClassToGroup(GRPUPID& group_id) {
		return shm_pool_.AddClassToGroup(group_id, T::TYPE, sizeof(T));
	}

	int Init(void* mem, size_t max_size, bool fresh = true) {
		return shm_pool_.Init(mem, max_size, fresh);
	}

	template <typename T>	
	int CreateObjct(ShmPointer<T>shm_pointer) {
		 void* obj =  shm_pool_.NewObject(T::TYPE,  &shm_pointer.obj_id);
		 if (obj == NULL) {
		 	return -1;
		 }

		 shm_pointer.obj_ptr = dynamic_cast<T*>(obj);
		 return 0;
	}
		
	template <typename T>	
	void FreeObject(shmPointer<T>& shm_pointer) {
		shm_pool_.FreeObject(shm_pointer.obj_id);
	}

public:
	friend class template <class T> ShmPointer;
private:	
	template <class T>	
	int Recovery(ShmPointer<T>& shm_pointer) {
		return 0;
	}

	ObjectCreatorMgr* obj_creator_mgr_;
	ShmPool shm_pool_;
};


/// 共享内存指针
template <class T>
class ShmPointer {
public:
	friend class ShmObjMgr;
	void Recovery() {
		ShmObjMgr::Singleton().Recovery<T>(*this);	
	}
private: 
	T* obj_ptr; 
	ObjID obj_id;
};

/// interface;
class ShmObj {
public:	
	virtual void recovery();
};

class ClassB: class ShmObj {
public:
};

class ClassA: class ShmObj{
public:
	virtual void Recovery() {		
		bptr->xxx();
	}	
private:	
	ShmPointer<ClassB> bptr;	
};

#endif // SHM_OBJ_MGR_H_


