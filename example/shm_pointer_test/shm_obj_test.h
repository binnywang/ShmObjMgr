#ifndef SHM_OBJ_TEST_H_
#define SHM_OBJ_TEST_H_

#include "shm_pointer.h"
#include "weak_shm_pointer.h"
#include "enable_shm_pointer_from_this.h"

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
	}

	virtual void Recover() {
		weak_shm_poiner_.Recover();
	}

	virtual void Update();

	void SetMember(int member) {member_ = member;}

	void SetWeakShmPointerOfClassB(const WeakShmPointer<ClassB> &weak_shm_poiner) {
		weak_shm_poiner_ = weak_shm_poiner;
	}

	~ClassA() {
		std::cout << "~ClassA()" << std::endl;
	}
	
private:
	WeakShmPointer<ClassB> weak_shm_poiner_;
private:
	int member_;
	
};


class ClassB;

class Foo {
public:
	void SetOwner(const WeakShmPointer<ClassB>& owner) {
		owner_ = owner;
	}

	virtual void Update();

	virtual void Recover() {
		owner_.Recover();

	}
private:
	WeakShmPointer<ClassB> owner_; 
};


class ClassB: public Base, public EnableShmPointerFromThis<ClassB> {
public:
	DeclareTypeName(ClassB);

	ClassB() {
		std::cout << "ClassB()" << std::endl;
	}

	~ClassB() {
		std::cout << "~ClassB()" << std::endl;
	}

	virtual void Recover() {
		EnableShmPointerFromThis<ClassB>::Recover();
		shm_poiner_.Recover();
		foo_.Recover();
	}

	void Callback() {
		std::cout << "ClassB Callback()" << std::endl;
	}
	
	virtual void Update();
	
	void SetMember(int member) {member_ = member;}

	void SetShmPointerOfClassA(const ShmPointer<ClassA> &shm_poiner) {
		shm_poiner_ = shm_poiner;
	}

private:
	ShmPointer<ClassA> shm_poiner_;
	Foo  foo_;
private:
	int member_;
};

void Foo::Update() {
	if (! owner_.Expired()) {
		owner_.Lock()->Callback();
	}
}

void ClassA::Update() {
	std::cout << "class A update with member_" << std::dec << member_ <<  std::endl;
	if (!weak_shm_poiner_.Expired()) {
		weak_shm_poiner_.Lock()->Update();
	}
}

void ClassB::Update() {
	std::cout << "class B update with member_" << std::dec << member_ << std::endl;
	foo_.SetOwner(WeakShmPointer<ClassB>(ShmPointerFromThis()));
	foo_.Update();
}


ImplmentTypeName(Base);
ImplmentTypeName(ClassA);
ImplmentTypeName(ClassB);

#endif
