#ifndef WEAK_SHM_POINTER_H_
#define WEAK_SHM_POINTER_H_
#include "shm_obj_mgr.h"

template <typename T> class ShmPointer;

template <typename T>
class WeakShmPointer: public BasePointer<T> {
public: 
	friend class ShmPointer<T>;
	
	using BasePointer<T>::UseCount;

	WeakShmPointer(): BasePointer<T>() {LOG_DEBUG("WeakShmPointer\n");}
	explicit WeakShmPointer(T* ptr, const ShmPool::ObjID& id): BasePointer<T>(ptr, id) {}
	explicit WeakShmPointer(const ShmPointer<T>& shm_pointer):BasePointer<T>(shm_pointer.obj_ptr, shm_pointer.obj_id) {}

	WeakShmPointer(const WeakShmPointer<T>& other) : BasePointer<T>(other.obj_ptr, other.obj_id) {}
	bool Expired() const {
		return UseCount() == 0;
	}

	WeakShmPointer<T>& operator=(const WeakShmPointer<T>& other) {
		this->obj_ptr = other.obj_ptr;
		this->obj_id = other.obj_id;
	}
	
	ShmPointer<T> Lock() const {
		return Expired() ? ShmPointer<T>() : ShmPointer<T>(*this);
	}
};


#endif //
