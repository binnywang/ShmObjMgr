#ifndef SHM_POINTER_H_
#define SHM_POINTER_H_

#include "shm_obj_mgr.h"

namespace shm_obj {

template <typename T> class WeakShmPointer;
template <typename T> class ShmPointer;
template <typename T> class EnableShmPointerFromThis;

template <typename T, typename Y> inline void enable_shm_pointer_from_this( ShmPointer<Y> const * ppx,  ShmObjPtr<T> const* p, const EnableShmPointerFromThis<T> * pe ) {
    if( pe != 0 ) {
        pe->_internal_accept_owner(ppx, p);
    }
}

inline void enable_shm_pointer_from_this(...) {
	
}

template <typename T> inline void shm_pointer_construct( ShmPointer<T> * const ppx, ShmObjPtr<T> const *p) {
	enable_shm_pointer_from_this(ppx, p, p->Get());
}


template <typename T>
class ShmPointer {
public: 
	ShmPointer() {}
	virtual ~ShmPointer() { Reset();}

	template <typename Y>
	explicit ShmPointer(const ShmObjPtr<Y>& p) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = p;
		count_ptr_ = p;
		if (count_ptr_) {
			count_ptr_->IncUseCount();
		}
		shm_pointer_construct(this, &obj_ptr_);
	}

	template <typename Y> 
	explicit ShmPointer(const WeakShmPointer<Y>& r) : count_ptr_(r.count_ptr_) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
		if (count_ptr_) {
			count_ptr_->IncUseCount();
		}
	}
	
	template <typename Y> 
	explicit ShmPointer(const ShmPointer<Y>& r) { 
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
		count_ptr_ = r.count_pt_;
		if (count_ptr_) {
			count_ptr_->IncUseCount();
		}
	}

	///aliasing
	template <typename Y> 
	ShmPointer(const ShmPointer<Y>& r, const ShmObjPtr<T>& p) { 
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = p;
		count_ptr_ = r.count_ptr_;
		if (count_ptr_) {
			count_ptr_->IncUseCount();
		}
	}

	template<typename Y>
	ShmPointer& operator=(const ShmPointer<Y>& r) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;

		if (r.count_ptr_) {
			r.count_ptr_->IncUseCount();
		}

		if (count_ptr_) {
			count_ptr_->DecUseCount();
			if (count_ptr_->UseCount() == 0) {
				ShmObjMgr::Instance().FreeObject(count_ptr_);
			}
		}

		count_ptr_ = r.count_ptr_;
		return *this;
	}

	void Reset() {
		if (count_ptr_) {
			count_ptr_->DecUseCount();
			if (count_ptr_->UseCount() == 0) {
				ShmObjMgr::Instance().FreeObject(count_ptr_);
			}
		}
		obj_ptr_.Reset();
		count_ptr_.Reset();
	}

	int Recover() {
		int ret = ShmObjMgr::Instance().Recover(obj_ptr_);
		if (ret) {
			count_ptr_.Reset();
			obj_ptr_.Reset();
			return ret;
		}
		count_ptr_ = obj_ptr_;
		return 0;
	}

	size_t UseCount() const { return count_ptr_ ? count_ptr_->UseCount() : 0; }

	operator bool() const { return obj_ptr_; }
	
	T* operator->() const { return obj_ptr_.operator->(); }

	T& operator*() const { return obj_ptr_.operator*(); }

	T* Get() const { return obj_ptr_.Get(); }

	const ShmObjPtr<T> & GetObjPtr() const { return obj_ptr_;}

	/// for debug
	template <typename Y>
	void SetObjPtr(const ShmObjPtr<Y>& p) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = p;
		count_ptr_ = obj_ptr_;
	} 

private:
	ShmObjPtr<ShmObj> count_ptr_; /// 计数器类
	ShmObjPtr<T>   obj_ptr_;
private:
	template <typename Y> friend class WeakShmPointer;
	template <typename Y> friend class ShmPointer;
};


template <typename T, typename U> inline bool operator==(ShmPointer<T> const & a, ShmPointer<U> const & b) {
    return a.Get() == b.Get();
}

template <typename T, typename U> inline bool operator!=(ShmPointer<T> const & a, ShmPointer<U> const & b) {
    return a.Get() != b.Get();
}

template <typename T, typename U> ShmPointer<T> static_pointer_cast( ShmPointer<U> const & r ) {
    (void) static_cast< ShmObjPtr<T> * >( static_cast< ShmObjPtr<U> * >( 0 ) );

    ShmObjPtr<T> p = static_obj_ptr_cast(r.GetObjPtr());
    return ShmPointer<T>( r,  p);
}

template <typename T, typename U> ShmPointer<T> const_pointer_cast( ShmPointer<U> const & r ) {
    (void) const_cast< ShmObjPtr<T>* >( static_cast< ShmObjPtr<U>* >( 0 ) );

   ShmObjPtr<T> p = const_obj_ptr_cast(r.GetObjPtr());

    return ShmPointer<T>( r, p );
}

template <typename T, typename U> ShmPointer<T> dynamic_pointer_cast( ShmPointer<U> const & r ) {
    (void) dynamic_cast< ShmObjPtr<T>* >( static_cast< ShmObjPtr<U>* >( 0 ) );
  	
  	ShmObjPtr<T> p = dynamic_obj_ptr_cast(r.GetObjPtr());
    return p? ShmPointer<T>( r, p ): ShmPointer<T>();
}

template <typename T, typename U> ShmPointer<T> reinterpret_pointer_cast( ShmPointer<U> const & r ) {
    (void) reinterpret_cast< ShmObjPtr<T>* >( static_cast< ShmObjPtr<U>* >( 0 ) );

   	ShmObjPtr<T> p = reinterpret_obj_ptr_cast(r.GetObjPtr());
    return ShmPointer<T>( r, p );
}

} //namespace
#endif	/// SHM_POINTER_H_
