#ifndef SHM_OBJ_TEST_H_
#define SHM_OBJ_TEST_H_

#include "shm_obj_mgr.h"

using namespace shm_obj;

/// 普通类
class XXManager {
public:
	void Update() {
		std::cout << "XXManager update" << std::endl;
	} 
};

XXManager globalXXManager;

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

	///  
	virtual void Recover() {
		xx_manager_ = &globalXXManager;
		if (xx_manager_) {
			xx_manager_->Update();
		}
	}
	
	virtual void Update();

	void SetMember(int member) {
		member_ = member;
	}

	void SetXXManager(XXManager* xx_manager) {
		xx_manager_ = xx_manager;
	}

	~ClassA() {
		std::cout << "~ClassA()" << std::endl;
	}
	
private:
	int member_;
private:
	XXManager* xx_manager_;
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
	if (xx_manager_) {
		xx_manager_->Update();
	}
} 

void ClassB::Update() {
	std::cout << "class B update with member_ = " << member_ <<  std::endl;
	if (a_ptr_) {
		a_ptr_->Update();
	}
} 

ImplmentTypeName(Base);
ImplmentTypeName(ClassA);
ImplmentTypeName(ClassB);
#endif
