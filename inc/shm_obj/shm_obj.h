#ifndef SHM_OBJ_H_
#define SHM_OBJ_H_

#include <stdint.h>
#include <stddef.h>

#include "system.h"
#include "log.h"

namespace shm_obj {

/// 共享内存对象基类
class ShmObj {
public:
	ShmObj() {
		if (System::IsResume()) {
			return;
		}
		use_count_ = 0;
	}

	virtual void Recover() {}

	virtual ~ShmObj() { LOG_DEBUG("~ShmObj"); use_count_ = 0; }

	inline size_t IncUseCount() {
		return ++use_count_;
	}

	inline size_t DecUseCount() {
		return --use_count_;
	}

	inline size_t UseCount() {
		return use_count_;
	}
private:
	size_t use_count_;
};

typedef uint8_t GroupId;
typedef uint8_t ClassId;

/// 共享内存对象Id
typedef union ObjId {
	uint64_t id;
	struct {
		GroupId group_id;
		ClassId class_id;
		uint16_t seq;
		int32_t index;
	};
} ObjId;

/// Y类型是否可以隐试转换为T类型
template <typename Y, typename T> inline void shm_pointer_assert_convertible() {
	T *p = static_cast<Y*>(0);
	(void)p;
}

/// 共享内存指针封装
template <typename T> class ShmObjPtr;
class ShmObjMgr;

template <typename T>
class ShmObjPtr {
public:
	ShmObjPtr() {
		if(System::IsResume()) {
			return;
		} else {
			Reset();
		}
	}

	ShmObjPtr(T *obj_ptr, ObjId obj_id) : obj_ptr_(obj_ptr), obj_id_(obj_id) {}

	~ShmObjPtr() {
		Reset();	
	}

	template <typename Y> ShmObjPtr(const ShmObjPtr<Y> & r) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
		obj_id_ = r.obj_id_;
	}

	template <typename Y> const ShmObjPtr<T>& operator=(const ShmObjPtr<Y> & r) {
		shm_pointer_assert_convertible<Y, T>();
		obj_ptr_ = r.obj_ptr_;
		obj_id_ = r.obj_id_;
		return *this;
	}	

	void Reset() {
		obj_ptr_ = 0;
		obj_id_.id = 0;
	}

	const ObjId& obj_id() const { return obj_id_;}

 	/// only for debug
 	void set_obj_id(const ObjId& obj_id) { obj_id_ = obj_id;}

 	T* Get() const { return obj_ptr_; }

	operator bool() const;
	
	T* operator->() const { return static_cast<T*>(this->obj_ptr_); }

	T& operator*() const { return *static_cast<T*>(this->obj_ptr_); }

private:
	mutable T* obj_ptr_;
	ObjId obj_id_;

	friend class ShmObjMgr;

	template<typename Y> friend class ShmObjPtr;
};

template<typename T, typename U> inline bool operator==(ShmObjPtr<T> const & a, ShmObjPtr<U> const & b) {
	return a.obj_id() == b.obj_id();
}

template<typename T, typename U> inline bool operator!=(ShmObjPtr<T> const & a, ShmObjPtr<U> const & b) {
	return a.obj_id() != b.obj_id();
}

template<typename T, typename U> ShmObjPtr<T> static_obj_ptr_cast( ShmObjPtr<U> const & r ) {
    (void) static_cast< T* >( static_cast< U* >( 0 ) );
   	
   	return  ShmObjPtr<T>(static_cast< T* >( r.Get()) , r.obj_id());
}

template<typename T, typename U> ShmObjPtr<T> const_obj_ptr_cast( ShmObjPtr<U> const & r ) {
    (void) const_cast< T* >( static_cast< U* >( 0 ) );

    return  ShmObjPtr<T>(const_cast< T* >( r.Get()), r.obj_id());
}

template<typename T, typename U> ShmObjPtr<T> dynamic_obj_ptr_cast( ShmObjPtr<U> const & r ) {
    (void) dynamic_cast< T* >( static_cast< U* >( 0 ) );
    
    return  ShmObjPtr<T>(dynamic_cast< T* >( r.Get()), r.obj_id());
}

template<typename T, typename U> ShmObjPtr<T> reinterpret_obj_ptr_cast( ShmObjPtr<U> const & r ) {
    (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

    return  ShmObjPtr<T>(reinterpret_cast< T* >( r.Get()), r.obj_id());
}


} // namespace 

#endif // SHM_OBJ_H_