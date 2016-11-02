#ifndef SHM_OBJ_MGR_H_
#define SHM_OBJ_MGR_H_

#include "shm_pool.h"
#include "singleton.h"
#include "log.h"

#include "shm_obj.h"

namespace shm_obj {

class ShmObjMgr : public Singleton<ShmObjMgr> {
public:		
	DeclareSingleton(ShmObjMgr);
public:

	int NewClassGroup(GroupId& group_id) {
		return shm_pool_.NewClassGroup(group_id);
	}
	
	template<typename T>
	int AddClassToGroup(GroupId& group_id, size_t max_obj_count) {
		return shm_pool_.AddClassToGroup(group_id, T::TYPE, sizeof(T), max_obj_count);
	}

	size_t TotalSize() const {
		return shm_pool_.TotalSize();
	}

	int Init(void* mem, size_t max_size, bool fresh = true) {
		return shm_pool_.Init(mem, max_size, fresh);
	}

	template <typename T>
	ShmObjPtr<T> CreateObject() {
		ObjId obj_id;
		void* obj =  shm_pool_.NewObject(T::TYPE, obj_id);
		if (obj == NULL) {
			return ShmObjPtr<T>();
		}
		return ShmObjPtr<T>(reinterpret_cast<T*>(obj), obj_id);
	}

	template<typename T>
	void FreeObject(const ShmObjPtr<T>& p) {
		shm_pool_.FreeObject(p.obj_id());
	}

	template<typename T>
	int Recover(ShmObjPtr<T>& p) {
		void* obj = shm_pool_.GetObject(p.obj_id());
		if (obj == NULL) {
			LOG_ERROR("recovery pointer error|obj_id[%zu]\n", p.obj_id().id);
			return -1;
		}
		
		p.obj_ptr_ = static_cast<T*>(reinterpret_cast<ShmObj*>(obj));
		return 0;
	}

private:
	ShmPool shm_pool_;
};

/// FIXME: 如何解决模板类循环引用?
template <typename T>
ShmObjPtr<T>::operator bool() const {
	if (obj_id_.id == 0) {
		return false;
	}
	int ret = ShmObjMgr::Instance().Recover(*const_cast<ShmObjPtr<T> *>(this));
	if (ret != 0) {
		return false;
	}
	return true;
}

} //namespace

#endif // SHM_OBJ_MGR_H_


