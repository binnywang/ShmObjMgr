#ifndef SHM_POOL_H_
#define SHM_POOL_H_

#include <vector> 
#include <stdint.h>

static const int MAX_CLASS_PER_GROUP = 10;
static const int MAX_CLASS_GROUP_NUM = 10;

typedef int GRPUPID;

class ClassGroupInfo {
public:
	GRPUPID group_id;
	int    class_id_array[MAX_CLASS_PER_GROUP];
	size_t max_class_size;
	size_t max_obj_count;
};

class ClassGroupInfoArray {
	int group_num;
    ClassGroupInfo class_groups[MAX_CLASS_GROUP_NUM];
};

class ShmPool {

public:
	static const int MAX_CLASS_PER_GROUP = 10;
	static const int MAX_CLASS_GROUP_NUM = 10;

///对象实例Id
	typedef union ObjID {
		uint64_t obj_id;
		struct {
			uint32_t group_id;
			uint32_t obj_index;
		};
	} ObjID;
	


///每一组对象的信息	
	struct ObjIndex {
		/// ObjID obj_id;
		int class_id;
		int index;
		int next;
		int prev;
	};

	typedef char ObjNode;

	struct ObjGroup {
		GRPUPID group_id;
		size_t group_size;
		size_t max_class_size;
		unsigned int max_obj_count;
		unsigned int used_obj_count;
		int class_id_num;
		int class_id[MAX_CLASS_PER_GROUP];
		ObjIndex* obj_index_;
		ObjNode* obj_node_;
		int free_index_head;
		int used_index_head;
	};

/// 共享内存头，包含所有的分组信息
	struct ClassGroupUnit {
		GRPUPID group_id;
		int class_id_num;
		int class_id[MAX_CLASS_PER_GROUP];
		size_t max_class_size;
		size_t max_obj_count;
		ObjGroup* obj_group;
	};

	struct ClassGroupInfo {
		int group_num;
    	ClassGroupUnit group[MAX_CLASS_GROUP_NUM];
	};

	struct ShmPoolHeader {
		size_t max_size;
		ClassGroupInfo group_info;
	};

public:
	ShmPool();

public:
	int NewClassGroup(GRPUPID& group_id);
	int AddClassToGroup(GRPUPID& group_id, int class_id, size_t class_size, size_t obj_count);
	size_t TotalSize();
	int Init(void* mem, size_t max_size, bool fresh = true);
	void* NewObject(int class_id, ObjID& obj_id);
	void* GetObject(const ObjID& obj_id);
	void FreeObject(const ObjID& obj_id);
private:
	void InitObjGroup(ObjGroup* obj_group);
	ObjGroup* FindGroupByClassId(int class_id);
	ObjGroup* FindGroupByGroupId(int group_id);
	void* NewObject(ObjGroup* obj_group, int class_id, ObjID & obj_id);
	void* GetObject(ObjGroup* obj_group, const ObjID & obj_id);
	void FreeObject(ObjGroup* obj_group, const ObjID & obj_id);
	inline unsigned int GeneratorNewID() {
		return id_generator_++;
	}
	
	ClassGroupInfo tmp_group_info_;
	ShmPoolHeader* shm_pool_header_;
	unsigned int id_generator_;
};


#endif
