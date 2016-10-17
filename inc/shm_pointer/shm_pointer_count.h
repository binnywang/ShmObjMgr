#ifndef SHM_POINTER_COUNT_H_
#define SHM_POINTER_COUNT_H_

#include "log.h"
#include "shm_obj.h"
#include "obj_creator_mgr.h"
#include "shm_obj_mgr.h"

#include <assert.h>

namespace shm_obj {

class ShmObjMgr;

class ShmObjCount : public ShmObj {
public:
	DeclareTypeName(ShmObjCount);
public:
	ShmObjCount()  {
		if(System::IsResume()) {
			return;
		} else {
			use_count_ = 1;
			weak_count_ = 1;
		} 
	}

	~ShmObjCount() {LOG_DEBUG("~ShmObjCount\n");}

private:
	inline size_t IncUseCount() {
		return ++use_count_;
	}

	inline size_t DecUseCount() {
		return --use_count_;
	}

	inline size_t IncWeakCount() {
		return ++weak_count_;
	}

	inline size_t DecWeakCount() {
		return --weak_count_;
	}

	inline size_t WeakCount() {
		return weak_count_;
	}

public:
	void SetShmObjPtr(const ShmObjPtr<ShmObj> & obj_ptr) {
		shm_obj_ptr_ = obj_ptr;
	}

	void SetSelfPtr(const ShmObjPtr<ShmObjCount> & self_ptr) {
		self_ptr_ = self_ptr;
	}
private:
	/// 释放对象
	void Dispose() {
		ShmObjMgr::Instance().FreeObject(shm_obj_ptr_);
	}

	/// 释放自身
	void Destroy() {
		ShmObjMgr::Instance().FreeObject(self_ptr_);
	}

public:

	inline size_t UseCount() {
		return use_count_;
	}

	void AddRef() {
		IncUseCount();
	}

	void Release() {
		DecUseCount();
		if(UseCount() == 0) {
			Dispose();
			WeakRelease();
		}
	}

	void WeakAddRef() {
		IncWeakCount();
	}

	void WeakRelease() {
		DecWeakCount();
		if (WeakCount() == 0) {
			Destroy();
		}
	}

	virtual int Recover() {
		return 0;
	}

	void Reset() {
		shm_obj_ptr_.Reset();
	}

private:
	size_t use_count_;
	size_t weak_count_;

private:
	ShmObjPtr<ShmObj> shm_obj_ptr_;
	ShmObjPtr<ShmObjCount> self_ptr_; /// for free self
};

class WeakShmObjCountPtr;

class ShmObjCountPtr {
public:

	friend class WeakShmObjCountPtr;

	ShmObjCountPtr() { LOG_DEBUG("ShmPointerCount\n");}
	virtual ~ShmObjCountPtr() {
		LOG_DEBUG("~ShmObjCountPtr\n"); 
		Reset();
	}

	template <typename T>
	ShmObjCountPtr(const ShmObjPtr<T>& p) {
		obj_count_ptr_ = ShmObjMgr::Instance().CreateObject<ShmObjCount>();
		if (! obj_count_ptr_) {
			LOG_ERROR("%s: Create ShmObjCount Error, Free ShmObj[%zu]\n", __FUNCTION__,  p.obj_id().id);
			ShmObjMgr::Instance().FreeObject(p);
			assert(obj_count_ptr_);
		}

		obj_count_ptr_->SetShmObjPtr(p);
		obj_count_ptr_->SetSelfPtr(obj_count_ptr_);
	}

	template <typename T>
	int Init(const ShmObjPtr<T> &p) {
		obj_count_ptr_ = ShmObjMgr::Instance().CreateObject<ShmObjCount>();
		if (! obj_count_ptr_) {
			LOG_ERROR("%s: Create ShmObjCount Error, Free ShmObj[%zu]\n", __FUNCTION__,  p.obj_id().id);
			return -1;
		} else {
			obj_count_ptr_->SetShmObjPtr(p);
			obj_count_ptr_->SetSelfPtr(obj_count_ptr_);
		}
		return 0;
	}

	ShmObjCountPtr(const ShmObjCountPtr& r) : obj_count_ptr_(r.obj_count_ptr_) {
		if (obj_count_ptr_) {
			obj_count_ptr_->AddRef();
		}
	}

	explicit ShmObjCountPtr(const WeakShmObjCountPtr& r);

	ShmObjCountPtr& operator=(const ShmObjCountPtr& r) {
		if (r.obj_count_ptr_) {
			r.obj_count_ptr_->AddRef();
		}

		if (obj_count_ptr_) {
			obj_count_ptr_->Release();
		}

		obj_count_ptr_ = r.obj_count_ptr_;
		return *this;

	}
	
	/// recover ob_ptr_ by obj_id_ when system  resume
	int Recover() {
		int ret = ShmObjMgr::Instance().Recover(obj_count_ptr_);
		if (ret != 0) {
			LOG_ERROR("%s: ShmObjCountPtr RecoverPtr error\n", __FUNCTION__);
			obj_count_ptr_.Reset();
		}
		return ret;
	}

	void Reset() {
		if (obj_count_ptr_) {
			obj_count_ptr_->Release();
			obj_count_ptr_.Reset();
		}
	}

	size_t UseCount() const {
		return obj_count_ptr_ ? obj_count_ptr_->UseCount() : 0;
	}

	/// for debug
	const ShmObjPtr<ShmObjCount> &  GetObjCountPtr() const {
		return obj_count_ptr_;
	}

	void SetObjCountPtr(const ShmObjPtr<ShmObjCount> & count_ptr) {
		obj_count_ptr_ = count_ptr;
	}

private:
	ShmObjPtr<ShmObjCount> obj_count_ptr_;
};


class WeakShmObjCountPtr {
public:

	friend class ShmObjCountPtr;

	WeakShmObjCountPtr() {	LOG_DEBUG("WeakShmObjCountPtr\n");}

	~WeakShmObjCountPtr() {
		LOG_DEBUG("~WeakShmObjCountPtr\n"); 
		Reset();
	}

	WeakShmObjCountPtr(const WeakShmObjCountPtr & r) : obj_count_ptr_(r.obj_count_ptr_) {
		if (obj_count_ptr_) {
			obj_count_ptr_->WeakAddRef();
		}
	}

	WeakShmObjCountPtr(const ShmObjCountPtr & r) : obj_count_ptr_(r.obj_count_ptr_) {
		if (obj_count_ptr_) {
			obj_count_ptr_->WeakAddRef();
		}
	}

	WeakShmObjCountPtr& operator=(const ShmObjCountPtr& r) {
		if (r.obj_count_ptr_) {
			r.obj_count_ptr_->WeakAddRef();
		}

		if (obj_count_ptr_) {
			obj_count_ptr_->WeakRelease();
		}

		obj_count_ptr_ = r.obj_count_ptr_;

		return *this;
	}

	WeakShmObjCountPtr& operator=(const WeakShmObjCountPtr& r) {
		if (r.obj_count_ptr_) {
			r.obj_count_ptr_->WeakAddRef();
		}

		if (obj_count_ptr_) {
			obj_count_ptr_->WeakRelease();
		}

		obj_count_ptr_ = r.obj_count_ptr_;
		return *this;
	}

	/// recover ob_ptr_ by obj_id_ when system  resume
	int Recover() {
		int ret = ShmObjMgr::Instance().Recover(obj_count_ptr_);
		if (ret != 0) {
			LOG_ERROR("%s: WeakShmObjCountPtr RecoverPtr error\n", __FUNCTION__);
			obj_count_ptr_.Reset();
		}
		return ret;
	}

	size_t UseCount() const {
		return obj_count_ptr_ ? obj_count_ptr_->UseCount() : 0;
	}

	void Reset() {
		if (obj_count_ptr_) {
			obj_count_ptr_->WeakRelease();
			obj_count_ptr_.Reset();
		}
	}

private:
	ShmObjPtr<ShmObjCount> obj_count_ptr_;
};

} //namespace

#endif /// SHM_POINTER_BASE_H_