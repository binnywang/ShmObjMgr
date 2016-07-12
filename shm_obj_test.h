#ifndef SHM_OBJ_TEST_H_
#define SHM_OBJ_TEST_H_

#include "shm_obj_mgr.h"
#include "obj_creator_mgr.h"

class Base: public ShmObj {
public:
	virtual void Update() {
		std::cout << "base update" << std::endl;
	}
};


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

	int Recovery() {
		std::cout << "recovery class A with member = " << GetMember() << std::endl;
	//	shm_poiner_.Recovery();
		return 0;
	}
	
	void Update() {
		member_++;
		std::cout << "class A update" << std::endl;
		//std::cout << "Class B with member = %d" << shm_poiner_->GetMember() << std::endl;
	}

	int GetMember() const { return member_;}
	void SetMember(int member) {member_ = member;}
	/*int SetShmPointer(const ShmPointer<ClassB> &shm_poiner) {
		shm_poiner_ = shm_poiner;
	}
	*/
	
private:
	//ShmPointer<ClassB> shm_poiner_;
private:
	int member_;
	
};

//ImplmentTypeName(ClassA);

class ClassB: public Base {
public:

	DeclareTypeName(ClassB);

	
	ClassB() {
		std::cout << "class B construtor" << std::endl;
	}

	ClassB(const ClassA & other) {
		std::cout << "class B copy construtor" << std::endl;
		member_ = other.GetMember();
	}

	int Recovery() {
		std::cout << "recovery class B with member = " << GetMember()  << std::endl;
		shm_poiner_.Recovery();
		return 0;
	}
	
	void Update() {
		member_++;
		std::cout << "class B update" << std::endl;
		std::cout << "class A with member = " << shm_poiner_->GetMember() << std::endl;
	}
	
	int GetMember() const { return member_;}
	void SetMember(int member) {member_ = member;}

	int SetShmPointer(const ShmPointer<ClassA> &shm_poiner) {
		shm_poiner_ = shm_poiner;
	}
private:
	ShmPointer<ClassA> shm_poiner_;
private:
	int member_;
};

#endif
