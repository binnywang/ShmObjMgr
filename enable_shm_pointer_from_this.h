#ifndef ENABLE_SHM_POINTER_FROM_THIS_H_
#define ENABLE_SHM_POINTER_FROM_THIS_H_

#include "shm_pointer.h"
#include "weak_shm_pointer.h"

#include <assert.h>

namespace shm_obj {

template <typename T>
class EnableShmPointerFromThis {
public:
	EnableShmPointerFromThis() {}
	~EnableShmPointerFromThis() {}

public:
	ShmPointer<T> ShmPointerFromThis() {
		ShmPointer<T> p(weak_this_);
		assert(p.Get() == this);
		return p;
	}

	ShmPointer<const T> ShmPointerFromThis() const {
		ShmPointer<const T> p(weak_this_);
		assert(p.Get() == this);
		return p;
	}	

	WeakShmPointer<T> WeakShmPointerFromThis() {
		return weak_this_;
	}

	WeakShmPointer<const T> WeakShmPointerFromThis() const {
		return weak_this_;
	}

	virtual int Recover() {
		return weak_this_.Recover();
	}
	
public:
	template <typename Y> 
	void _internal_accept_owner(ShmPointer<Y> const *ppx,  ShmObjPtr<T> const *py)  const {
		if (weak_this_.Expired()) {
			weak_this_ = ShmPointer<Y>(*ppx, *py);
		}
	}

private:
	mutable WeakShmPointer<T> weak_this_;	
};

} //namespace

#endif //ENABLE_SHM_PTR_FROM_THIS_H_