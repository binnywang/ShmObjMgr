#include "shm_pointer_count.h"

using namespace shm_obj;

ShmObjCountPtr::ShmObjCountPtr(const WeakShmObjCountPtr& r): obj_count_ptr_(r.obj_count_ptr_) {
	if (obj_count_ptr_) {
		obj_count_ptr_->AddRef();
	}
}