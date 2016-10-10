#ifndef SYSTEM_H_
#define SYSTEM_H_

class System {
public:
	static void SetSystemResume(bool flag) {
		resume_ = flag;
	}

	static bool IsResume() {
		return resume_;
	}
private:
	static bool resume_; 
private:
	System();
	~System();	
};

#endif //SYSTEM_H_