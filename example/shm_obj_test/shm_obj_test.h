#ifndef SHM_OBJ_TEST_H_
#define SHM_OBJ_TEST_H_

#include "shm_obj_mgr.h"

using namespace shm_obj;

class Base: public ShmObj {
public:
	DeclareTypeName(Base);
	virtual void Update() {
		std::cout << "base update" << std::endl;
	}
};

class ClassB;

class ClassA : public Base {
public:
	DeclareTypeName(ClassA);

	ClassA() {
		std::cout << "ClassA()" << std::endl;
		if (System::IsResume()) {
			return;
		}
		member_ = 0;
	}
	
	virtual void Update();

	void SetMember(int member) {
		member_ = member;
	}

	~ClassA() {
		std::cout << "~ClassA()" << std::endl;
	}
	
private:
	int member_;
};

class ClassB: public Base {
public:

	DeclareTypeName(ClassB);

	ClassB() {
		std::cout << "ClassB()" << std::endl;
		if (System::IsResume()) {
			return;
		}
		member_ = 0;
	}

	~ClassB() {
		std::cout << "~ClassB()" << std::endl;
	}

	virtual void Update();
	
	void SetClassAObjPtr(const ShmObjPtr<ClassA> &a_ptr) {
		a_ptr_ = a_ptr;
	}

	void SetMember(int member) {
		member_ = member;
	}

private:
	int member_;
	ShmObjPtr<ClassA> a_ptr_;
};


void ClassA::Update() {
	std::cout << "class A update with member_ = " << member_ << std::endl;
} 

void ClassB::Update() {
	std::cout << "class B update with member_ = " << member_ <<  std::endl;
	if (a_ptr_) {
		a_ptr_->Update();
	}
} 

#endif
