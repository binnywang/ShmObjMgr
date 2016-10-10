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

	~Base() {
		std::cout << "~Base" << std::endl;
	}
	
};

class ClassB;

class ClassA : public Base {
public:

	DeclareTypeName(ClassA);
	ClassA() {
		std::cout << "class A construtor" << std::endl;
	}

	ClassA(const ClassA & other) {
		std::cout << "class A copy construtor" << std::endl;
		member_ = other.GetMember();
	}

	int Recover() {
		std::cout << "recovery class A with member = " << std::dec <<  GetMember() << std::endl;
		weak_shm_poiner_.Recover();
		return 0;
	}
	
	virtual void Update();
	
	int GetMember() const { return member_;}
	void SetMember(int member) {member_ = member;}
	void SetShmPointer(const WeakShmPointer<ClassB> &weak_shm_poiner) {
		weak_shm_poiner_ = weak_shm_poiner;
		// std::cout << "weak_shm_pointer.obj_id = %d" << weak_shm_poiner_.obj_id().id << std::endl;
	}

	~ClassA() {
		std::cout << "~class A" << std::endl;
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

	void Update();

	void Recover() {
		owner_.Recover();

	}
private:
	WeakShmPointer<ClassB> owner_; 
};

//ImplmentTypeName(ClassA);

class ClassB: public Base , public EnableShmPointerFromThis<ClassB> {
public:

	DeclareTypeName(ClassB);

	
	ClassB() {
		std::cout << "class B construtor" << std::endl;
	}

	ClassB(const ClassA & other) {
		std::cout << "class B copy construtor" << std::endl;
		member_ = other.GetMember();
	}

	~ClassB() {
		std::cout << "~class B" << std::endl;
	}

	int Recover() {
		std::cout << "recovery class B with member = " << std::dec << GetMember()  << std::endl;
		EnableShmPointerFromThis<ClassB>::Recover();
		shm_poiner_.Recover();
		foo_.Recover();
		return 0;
	}

	void Speak() {
		std::cout << "class b speek with member = " << std::dec << GetMember() << std::endl;
	}
	
	virtual void Update();
	
	int GetMember() const { return member_;}
	void SetMember(int member) {member_ = member;}

	void SetShmPointer(const ShmPointer<ClassA> &shm_poiner) {
		shm_poiner_ = shm_poiner;
		//std::cout << "shm_pointer.obj_id = " << shm_poiner_.obj_id().obj_id << std::endl;
	}

private:
	ShmPointer<ClassA> shm_poiner_;
	Foo  foo_;
private:
	int member_;
};

void Foo::Update() {
		if (! owner_.Expired()) {
			owner_.Lock()->Speak();
		}
	}


void ClassA::Update() {
	std::cout << "class A update" << std::endl;
	if (!weak_shm_poiner_.Expired()) {
		//std::cout << "weak_pointer" << std::endl;
		weak_shm_poiner_.Lock()->Update();
	}
}

void ClassB::Update() {
	std::cout << "class B update" << std::endl;
	//ShmPointer<ClassB> owner = ShmPointerFromThis();
	foo_.SetOwner(WeakShmPointer<ClassB>(ShmPointerFromThis()));
	foo_.Update();
	/*
	if (shm_poiner_) {
		shm_poiner_->Update();
	}
	*/
}


#endif
