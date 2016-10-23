#ifndef SHM_POOL_H_
#define SHM_POOL_H_

#include <stdint.h>
#include <stddef.h>
#include <sstream>
#include <string>

#include "shm_obj.h"


namespace shm_obj {

class ShmPool {
public:
	static const int MAX_CLASS_PER_GROUP = 255;
	static const int MAX_CLASS_GROUP_NUM = 255;
	
	/// 每一组对象的信息	
	struct ObjIndex {
		ObjId obj_id;
		int32_t next;
		int32_t prev;
	};

	typedef char ObjNode;

	struct ObjGroup {
		GroupId group_id;
		size_t group_size;
		size_t max_class_size;
		unsigned int max_obj_count;
		unsigned int used_obj_count;
		int class_id_num;
		ClassId class_id[MAX_CLASS_PER_GROUP];
		ObjIndex* obj_index_;
		ObjNode* obj_node_;
		int free_index_head;
		int used_index_head;
		uint16_t seq;
	};

	/// 共享内存头，包含所有的分组信息
	struct ClassGroupUnit {
		GroupId group_id;
		int class_id_num;
		ClassId class_id[MAX_CLASS_PER_GROUP];
		size_t max_class_size;
		unsigned int max_obj_count;
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
	int NewClassGroup(GroupId& group_id);
	int AddClassToGroup(GroupId& group_id, ClassId class_id, size_t class_size, size_t obj_count);
	size_t TotalSize() const;
	int Init(void* mem, size_t max_size, bool fresh = true);
	void* NewObject(ClassId class_id, ObjId& obj_id);
	void* GetObject(const ObjId& obj_id);
	void FreeObject(const ObjId& obj_id);
	std::string DumpObj();

private:
	void InitObjGroup(ObjGroup* obj_group);
	std::string DumpObjGroup(ObjGroup* obj_group);

	ObjGroup* FindGroupByClassId(ClassId class_id);
	ObjGroup* FindGroupByGroupId(GroupId group_id);
	void* NewObject(ObjGroup* obj_group, ClassId class_id, ObjId & obj_id);
	void* GetObject(ObjGroup* obj_group, const ObjId & obj_id);
	void FreeObject(ObjGroup* obj_group, const ObjId & obj_id);
	
	ClassGroupInfo tmp_group_info_;
	ShmPoolHeader* shm_pool_header_;
};

} //namespace

#endif // SHM_POOL_H_ 
