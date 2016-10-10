#ifndef WEAK_SHM_POINTER_H_
#define WEAK_SHM_POINTER_H_

#include "shm_pointer_count.h"
#include "shm_obj_mgr.h"

namespace shm_obj {

template <typename T> class ShmPointer;
template <typename T> class WeakShmPointer;

template <typename T>
class WeakShmPointer {
public: 
	WeakShmPointer() {LOG_DEBUG("WeakShmPointer\n");}
	virtual ~WeakShmPointer() {LOG_DEBUG("~WeakShmPointer\n");}

	template <typename Y> 
	//WeakShmPointer(const WeakShmPointer<Y>& r) : ShmPointerBase(r.Lock().obj_ptr_, r.Lock().obj_id_) {
	WeakShmPointer(const WeakShmPointer<Y>& r) : count_ptr_(r.count_ptr_) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
	}

	template <typename Y> 
	explicit WeakShmPointer(const ShmPointer<Y>& r) : count_ptr_(r.count_ptr_) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
	}

	template <typename Y>
	WeakShmPointer& operator=(const WeakShmPointer<Y>& r) {
		shm_pointer_assert_convertible<Y, T>();
		count_ptr_ = r.count_ptr_;
		obj_ptr_ = r.obj_ptr_;
		return *this;
	}

	template <typename Y>
	WeakShmPointer& operator=(const ShmPointer<Y>& r) {
		shm_pointer_assert_convertible<Y, T>();
		count_ptr_ = r.count_ptr_;
		obj_ptr_ = r.obj_ptr_;
		return *this;
	}
		
	bool Expired() const {
		return  count_ptr_.UseCount() == 0;
	}

	ShmPointer<T> Lock() const {
		return Expired() ? ShmPointer<T>() : ShmPointer<T>(*this);
	}

	int Recover() {
		int ret = ShmObjMgr::Instance().Recover(obj_ptr_);
		if (ret != 0) {
			Reset();
		}
		return count_ptr_.Recover();
	}
	
	void Reset() {
		obj_ptr_.Reset();
		count_ptr_.Reset();
	}

private:
	WeakShmObjCountPtr count_ptr_;
	ShmObjPtr<T>   	   obj_ptr_;
private:
	template<typename Y> friend class WeakShmPointer;
	template<typename Y> friend class ShmPointer;

};

template<typename T, typename U> inline bool operator==(WeakShmPointer<T> const & a, WeakShmPointer<U> const & b) {
	return ShmPointer<T>(a) == ShmPointer<U>(b);
}

template<typename T, typename U> inline bool operator!=(WeakShmPointer<T> const & a, WeakShmPointer<U> const & b) {
	return ShmPointer<T>(a) != ShmPointer<U>(b);
}

}
#endif //WEAK_SHM_POINTER_H_
