
#ifndef SHM_OBJ_MGR_H_
#define SHM_OBJ_MGR_H_

#include "shm_pool.h"
#include "singleton.h"
#include "log.h"


template <typename T> class ShmPointer;

/// 共享内存指针
class ShmObj {
public:	
	template <typename T> friend class BasePointer;
	template <typename T> friend class ShmPointer;
	template <typename T> friend class WeakShmPointer;
public:
	//ShmObj(): {}
	
	virtual int Recovery() {return 0;} 

	virtual ~ShmObj() { use_count = 0; LOG_DEBUG("~ShmObj\n");}
private:
	size_t use_count;
};

class ShmObjMgr;

template <typename T>
class BasePointer {
public:

	friend class ShmObjMgr;

	BasePointer() { 

		LOG_DEBUG("BasePointer\n");
		if(!constructed) {
			Reset();
			constructed = true;
		} else {
			LOG_DEBUG("constructed\n");
		}
	}
	
	explicit BasePointer(T* ptr, const ShmPool::ObjID& id): obj_ptr(ptr), obj_id(id) {}
	virtual ~BasePointer() {LOG_DEBUG("~BasePointer\n");} 

	int Recovery();
	
	size_t UseCount() const {
		IsNull() ? 0 : dynamic_cast<ShmObj*>(obj_ptr)->use_count;
		LOG_DEBUG("obj_id = %lu, use_count = %d\n", obj_id.obj_id, dynamic_cast<ShmObj*>(obj_ptr)->use_count);
		return IsNull() ? 0 : dynamic_cast<ShmObj*>(obj_ptr)->use_count;
	}

	size_t IncUseCount() {
		IsNull() ? 0 : ++dynamic_cast<ShmObj*>(obj_ptr)->use_count;
		LOG_DEBUG("Inc:obj_id = %lu, use_count = %d\n", obj_id.obj_id, dynamic_cast<ShmObj*>(obj_ptr)->use_count);
		return IsNull() ? 0 : dynamic_cast<ShmObj*>(obj_ptr)->use_count;
	}

	size_t DecUseCount() {
		IsNull() ? 0 : --dynamic_cast<ShmObj*>(obj_ptr)->use_count;
		LOG_DEBUG("Dec: obj_id = %lu, use_count = %d\n", obj_id.obj_id, dynamic_cast<ShmObj*>(obj_ptr)->use_count);
		return IsNull() ? 0 : dynamic_cast<ShmObj*>(obj_ptr)->use_count;
	}

	const ShmPool::ObjID& GetObjID() const {
		return obj_id;
	}
	
	void SetObjID(ShmPool::ObjID& obj_id) {
		this->obj_id = obj_id;
	}

	void Reset() {
		obj_ptr = 0;
		obj_id.obj_id = 0;
	}

	bool IsNull() const { return obj_id.obj_id == 0 || obj_ptr == 0; }
protected:
	T* obj_ptr;
	ShmPool::ObjID obj_id;
private:
	bool constructed;
};


class ShmObjMgr : public Singleton<ShmObjMgr> {
public:		
	DeclareSingleton(ShmObjMgr);
public:

	int NewClassGroup(ShmPool::GRPUPID& group_id) {
		return shm_pool_.NewClassGroup(group_id);
	}
	
	template<typename T>
	int AddClassToGroup(ShmPool::GRPUPID& group_id, size_t max_obj_count) {
		return shm_pool_.AddClassToGroup(group_id, T::TYPE, sizeof(T), max_obj_count);
	}

	size_t TotalSize() const {
		return shm_pool_.TotalSize();
	}

	int Init(void* mem, size_t max_size, bool fresh = true) {
		return shm_pool_.Init(mem, max_size, fresh);
	}
/*
	template <typename DERIVE, typename BASE>	
	int CreateObject(ShmPointer<BASE>& shm_pointer) {
		 void* obj =  shm_pool_.NewObject(DERIVE::TYPE, shm_pointer.obj_id);
		 if (obj == NULL) {
		 	return -1;
		 }

		 shm_pointer.obj_ptr = dynamic_cast<BASE*>(reinterpret_cast<DERIVE*>(obj));
		 return 0;
	}
*/
	template <typename DERIVE, typename BASE>	
	ShmPointer<BASE> CreateObject() {
		ShmPool::ObjID obj_id;
		 void* obj =  shm_pool_.NewObject(DERIVE::TYPE, obj_id);
		 if (obj == NULL) {
		 	return ShmPointer<BASE>();
		 }

		 return ShmPointer<BASE>(dynamic_cast<BASE*>(reinterpret_cast<DERIVE*>(obj)), obj_id);
	}


	template <typename T>	
	void FreeObject(const ShmPointer<T>& shm_pointer) {
		shm_pool_.FreeObject(shm_pointer.GetObjID());
	}

/*
	template <typename T>	
	void FreeObject(const ShmPool::ObjID& obj_id) {
		shm_pool_.FreeObject(obj_id);
	}
	*/

	template <typename X> friend class BasePointer;
public:	
	
	template <typename T>
	int Recovery(BasePointer<T>& base_pointer) {
		void* obj = shm_pool_.GetObject(base_pointer.GetObjID());
		if (obj == NULL) {
			LOG_ERROR("recovery pointer error|obj_id[%lu]\n", base_pointer.GetObjID().obj_id);
			return -1;
		}

		base_pointer.obj_ptr = reinterpret_cast<T*>(obj);
		return 0;
	}

	ShmPool shm_pool_;
};

template <typename T>
int BasePointer<T>::Recovery() {
	int ret = ShmObjMgr::Instance().Recovery<T>(*this);
	if (ret != 0) {
		Reset();
	}

	return ret;
}


#endif // SHM_OBJ_MGR_H_


