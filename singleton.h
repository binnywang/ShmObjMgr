#ifndef SINGLETON_H_
#define SINGLETON_H_

template <typename T>
class Singleton {
public:
	static T& Instance() {
		return instance; 
	}

	static T* InstancePtr() {
		return &instance;
	}

protected:
	static T instance;
	ShmSingleton();
	~ShmSingleton();
	Singleton(const Singleton&);
	const Singleton & operator=(const Singleton&);
};

template <typename T>
T ShmSingleton<T>::instance;

#define DeclareSingleton(ClassName) public: friend class Singleton<ClassName>;

#endif //SHM_SINGLETON_H_
