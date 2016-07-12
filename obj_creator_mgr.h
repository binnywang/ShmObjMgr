#ifndef OBJ_CREATOR_MGR_H_
#define OBJ_CREATOR_MGR_H_

#include <new>
#include "singleton.h"

//using namespace std;
class IObjectCreator {
public:	
	virtual void* ReplacementNew(void* addr) = 0;	
	virtual void* New() = 0;
};

template <typename T>
class ObjectCreator: public IObjectCreator, 
					 public Singleton<ObjectCreator<T> > {
public:
	DeclareSingleton(ObjectCreator);
	
	virtual void* New() {
		return new T();
	}
	
	virtual void* ReplacementNew(void * addr) {
		return new(addr) T;	
	}
};

enum ClassTypeEnum {
	TYPE_MIN,
	TYPE_ClassA,	
	TYPE_ClassB,	
	MaxClassCount
};

#define DeclareTypeName(ClassName)	static const ClassTypeEnum TYPE = TYPE_##ClassName
//#define ImplmentTypeName(ClassName) const ClassTypeEnum ClassName::TYPE = ClassTypeEnum::ClassName


class ObjectCreatorMgr : public Singleton<ObjectCreatorMgr> {
public:
	DeclareSingleton(ObjectCreatorMgr);

public:
	template <typename T>	
	T* CreateObject(void* addr)	{
		IObjectCreator* creator = obj_creator_[T::TYPE];
		void* obj = creator->ReplacementNew(addr);
		return reinterpret_cast<T>(obj);
	}
	
	template <typename T> 
	int RegisterObjectCreator() {			
		obj_creator_[T::TYPE] = ObjectCreator<T>::InstancePtr();
		return 0;
	}

	void* CreateObject(void* addr, ClassTypeEnum class_id) {
		IObjectCreator* creator = obj_creator_[class_id];
		void* obj = creator->ReplacementNew(addr);
		return obj;
	}
private:
	IObjectCreator* obj_creator_[MaxClassCount];	
};

#endif //OBJ_CREATOR_MGR_H_