#include "shm_pool.h"
#include "log.h"
#include "system.h"
#include "obj_creator_mgr.h"

#include <time.h>
#include <string.h>

namespace shm_obj {

ShmPool::ShmPool() {
	memset(&tmp_group_info_, 0, sizeof(tmp_group_info_));
	shm_pool_header_ = NULL;
}

int ShmPool::NewClassGroup(GroupId & group_id) {
	if (tmp_group_info_.group_num >= MAX_CLASS_GROUP_NUM) {
		return -1;		
	}

	/// 直接用数组下标作为索引, 加快查找
	group_id = tmp_group_info_.group_num;
	tmp_group_info_.group[tmp_group_info_.group_num].group_id = group_id;
	tmp_group_info_.group_num++;
	return 0;
}

int ShmPool::AddClassToGroup(GroupId & group_id,ClassId class_id,size_t class_size,size_t obj_count) {
	if (group_id < 0 || group_id >= tmp_group_info_.group_num) {
		LOG_ERROR("cannot find group| group_id[%d]", group_id);
		return -1;
	}

	if (tmp_group_info_.group[group_id].class_id_num >= MAX_CLASS_PER_GROUP) {
		LOG_ERROR("group is full| group_id[%d]", group_id);
		return -2;
	}

	tmp_group_info_.group[group_id].class_id[tmp_group_info_.group[group_id].class_id_num] = class_id;
	tmp_group_info_.group[group_id].class_id_num++;
	tmp_group_info_.group[group_id].max_obj_count = tmp_group_info_.group[group_id].max_obj_count >= obj_count ? tmp_group_info_.group[group_id].max_obj_count : obj_count;
	tmp_group_info_.group[group_id].max_class_size = tmp_group_info_.group[group_id].max_class_size >= class_size ? tmp_group_info_.group[group_id].max_class_size : class_size;
	return 0;
}


size_t ShmPool::TotalSize() const {
	size_t total_size = 0;
	for (int i = 0; i < tmp_group_info_.group_num; i++) {
		total_size += sizeof(ObjGroup) + tmp_group_info_.group[i].max_obj_count * (sizeof(ObjIndex) + tmp_group_info_.group[i].max_class_size);
	}

	total_size += sizeof(ShmPoolHeader);
	return total_size;
}


int ShmPool::Init(void * mem,size_t max_size,bool fresh) {
	if (mem == NULL || max_size < TotalSize()) {
		LOG_ERROR("invalid param|mem[%p], max_size[%zu]", mem, max_size);
		return -1;
	}

	if (fresh) {
		LOG_INFO("Shm Pool Is Fresh\n");
		shm_pool_header_ = reinterpret_cast<ShmPoolHeader*>(mem);
		shm_pool_header_->max_size = TotalSize();
		shm_pool_header_->group_info = tmp_group_info_;

		ObjGroup* obj_group = reinterpret_cast<ObjGroup*>((char*)mem + sizeof(ShmPoolHeader));
		for(int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
			obj_group->group_id = shm_pool_header_->group_info.group[i].group_id;
			obj_group->max_obj_count = shm_pool_header_->group_info.group[i].max_obj_count;
			obj_group->max_class_size = shm_pool_header_->group_info.group[i].max_class_size;
			obj_group->group_size = sizeof(ObjGroup) + shm_pool_header_->group_info.group[i].max_obj_count * (shm_pool_header_->group_info.group[i].max_class_size + sizeof(ObjIndex));
			obj_group->class_id_num = shm_pool_header_->group_info.group[i].class_id_num;
			for (int j = 0; j < shm_pool_header_->group_info.group[i].class_id_num; j++) {
 				obj_group->class_id[j] = shm_pool_header_->group_info.group[i].class_id[j];
 			}
			shm_pool_header_->group_info.group[i].obj_group = obj_group;

 			InitObjGroup(obj_group);
			obj_group = reinterpret_cast<ObjGroup*>((char*)obj_group + obj_group->group_size);
 		}
	} else {
		LOG_INFO("Shm Pool Is Not Fresh, Begin Recover Set System Resume = true ================");
		System::SetSystemResume(true);
		/// recover
		shm_pool_header_ = reinterpret_cast<ShmPoolHeader*>(mem);
		if (shm_pool_header_->max_size != max_size) {
			LOG_ERROR("ShmPoolHeader MISMATCH|max_size [%zu, %zu] mismatch", shm_pool_header_->max_size, max_size);
			return -1;
		}

		ObjGroup* obj_group = reinterpret_cast<ObjGroup*>(((char*)mem + sizeof(ShmPoolHeader)));
		for (int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
			shm_pool_header_->group_info.group[i].obj_group = obj_group;

			if (obj_group->class_id_num != shm_pool_header_->group_info.group[i].class_id_num) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|class_id_num[%d, %d] mismatch", i, obj_group->class_id_num, shm_pool_header_->group_info.group[i].class_id_num);
				return -1;
			}

			for (int j = 0; j < obj_group->class_id_num; j++) {
				if (obj_group->class_id[j] != shm_pool_header_->group_info.group[i].class_id[j]) {
					LOG_ERROR("ObjGroup[%d] MISMATCH|class_id[%d] [%d, %d] mismatch", i, j, obj_group->class_id[j], shm_pool_header_->group_info.group[i].class_id[j]);
					return -1;
				}
			}

			if (obj_group->group_id != shm_pool_header_->group_info.group[i].group_id) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|group_id[%d, %d] mismatch", i, obj_group->group_id, shm_pool_header_->group_info.group[i].group_id);
				return -1;
			}

			if (obj_group->max_class_size != shm_pool_header_->group_info.group[i].max_class_size) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|max_class_size[%zu, %zu] mismatch", i, obj_group->max_class_size, shm_pool_header_->group_info.group[i].max_class_size);
				return -1;
			}

			if (obj_group->max_obj_count != shm_pool_header_->group_info.group[i].max_obj_count) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|max_obj_count[%u, %u] mismatch", i, obj_group->max_obj_count, shm_pool_header_->group_info.group[i].max_obj_count);
				return -1;
			}

			size_t group_size = sizeof(ObjGroup) + (sizeof(ObjIndex) +  obj_group->max_class_size) * obj_group->max_obj_count;
			if (obj_group->group_size != group_size) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|group_size[%zu, %zu] mismatch", i, obj_group->group_size, group_size);
				return -1;
			}

			obj_group->obj_index_ = reinterpret_cast<ObjIndex*>((char*)obj_group + sizeof(ObjGroup));
			obj_group->obj_node_ = reinterpret_cast<ObjNode*>((char*)obj_group->obj_index_ + sizeof(ObjIndex) * obj_group->max_obj_count);

			unsigned int used_obj_count = 0;
			int index = obj_group->used_index_head;
			while(index != -1) {
				used_obj_count++;
				void* obj = (void*)(obj_group->obj_node_ + index * obj_group->max_class_size);
				/// recovery obj;
				ShmObj* shm_obj = reinterpret_cast<ShmObj*>(ObjectCreatorMgr::Instance().CreateObject(obj, obj_group->obj_index_[index].obj_id.class_id));
				shm_obj->Recover();
				index = obj_group->obj_index_[index].next;
			}

			if (obj_group->used_obj_count != used_obj_count) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|used_obj_count[%d, %d] mismatch", i, obj_group->used_obj_count, used_obj_count);
				return -1;
			}
			unsigned int free_obj_count = 0;
			index = obj_group->free_index_head;
			while(index != -1) {
				free_obj_count++;
				index = obj_group->obj_index_[index].next;
			}

			if (obj_group->used_obj_count + free_obj_count != obj_group->max_obj_count) {
				LOG_ERROR("ObjGroup[%d] MISMATCH|max_obj_count[%d] used[%d] free[%d] mismatch", i, obj_group->max_obj_count, obj_group->used_obj_count, free_obj_count);
				return -1;
			}

			obj_group = reinterpret_cast<ObjGroup*>((char*)obj_group + obj_group->group_size);

		}
		LOG_INFO("========End Recover, Set System Resume = false");
		System::SetSystemResume(false);
	}

	LOG_INFO("ShmPool Init Success");

	return 0;
}

void ShmPool::InitObjGroup(ObjGroup * obj_group) {
	obj_group->obj_index_ = reinterpret_cast<ObjIndex*>((char*)obj_group + sizeof(ObjGroup));
	obj_group->obj_node_ = reinterpret_cast<ObjNode*>((char*)obj_group->obj_index_ + sizeof(ObjIndex) * obj_group->max_obj_count);
	obj_group->used_index_head = -1;
	obj_group->free_index_head = 0;
	obj_group->used_obj_count = 0;
	obj_group->seq = 0; ///初始可以为零，每次加一后再使用

	unsigned int i = 0;
	for( ; i < obj_group->max_obj_count - 1; i++) {
		obj_group->obj_index_[i].next = i + 1;
		obj_group->obj_index_[i].obj_id.id = 0;
	}
	
	obj_group->obj_index_[i].next = -1;
	obj_group->obj_index_[i].obj_id.id = 0;
}


ShmPool::ObjGroup* ShmPool::FindGroupByClassId(ClassId class_id) {
	for (int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
		for (int j = 0; j < shm_pool_header_->group_info.group[i].class_id_num; j++) {
			if (shm_pool_header_->group_info.group[i].class_id[j] == class_id) {
				return shm_pool_header_->group_info.group[i].obj_group;
			}
		}
	}
	return NULL;
}

ShmPool::ObjGroup* ShmPool::FindGroupByGroupId(GroupId group_id) {
	if (group_id < 0 || group_id >= shm_pool_header_->group_info.group_num) {
		return NULL;
	}

	return  shm_pool_header_->group_info.group[group_id].obj_group;
}

void* ShmPool::NewObject(ClassId class_id, ObjId & obj_id) {
	ObjGroup* obj_group = FindGroupByClassId(class_id);
	if (obj_group == NULL) {
		LOG_ERROR("cannot find obj_group for class_id[%d]", class_id);
		return NULL;
	}
	
	void* obj = NewObject(obj_group, class_id, obj_id);
	if (obj) {
		return ObjectCreatorMgr::Instance().CreateObject(obj, class_id);
	}  else {
		return NULL;
	}
}

void* ShmPool::GetObject(const ObjId & obj_id) {
	ObjGroup* obj_group = FindGroupByGroupId(obj_id.group_id);
	if (obj_group == NULL) {
		return NULL;
	}
	
	return GetObject(obj_group, obj_id);
}

void ShmPool::FreeObject(const ObjId & obj_id) {
	ObjGroup* obj_group = FindGroupByGroupId(obj_id.group_id);
	if (obj_group == NULL) {
		LOG_ERROR("cannot find obj_group for group_id[%d]", obj_id.group_id);
		return;
	}
	FreeObject(obj_group, obj_id);
}

void* ShmPool::NewObject(ObjGroup* obj_group, ClassId class_id, ObjId & obj_id) {
	if (obj_group->free_index_head == -1 || obj_group->used_obj_count >= obj_group->max_obj_count) {
		LOG_ERROR("obj_group used up| group_id[%d] free_index_head[%d] max_obj_count[%d] used_obj_count[%d]",
			obj_group->group_id, obj_group->free_index_head, obj_group->max_obj_count, obj_group->used_obj_count);
		return NULL;
	}

	int index = obj_group->free_index_head;
	obj_group->free_index_head = obj_group->obj_index_[index].next;
	
	obj_group->obj_index_[index].next = obj_group->used_index_head;
	obj_group->used_index_head = index;
	
	obj_group->used_obj_count++;
	obj_group->seq++;
	if(obj_group->seq == 0) {
		obj_group->seq++;
	}

	obj_id.group_id = obj_group->group_id;
	obj_id.index = index;
	obj_id.class_id = class_id;
	obj_id.seq = obj_group->seq;

	obj_group->obj_index_[index].obj_id = obj_id;

	void* obj =(void *)(obj_group->obj_node_ + index * obj_group->max_class_size);
	/// NOTE: important!!!
	memset(obj, 0, obj_group->max_class_size);

	return obj;
}

void* ShmPool::GetObject(ObjGroup* obj_group, const ObjId & obj_id) {
	if (obj_group->used_obj_count <= 0 || obj_group->used_index_head == -1) {
		LOG_ERROR("obj_group empty|group_id[%d]", obj_group->group_id);
		return NULL;
	}

	/*
	int index = obj_group->used_index_head;
	while (index != -1) {
		if (index == obj_id.index) {
			break;
		}
		index = obj_group->obj_index_[index].next;
	}
	
	if (index != obj_id.index) {
		LOG_ERROR("%s:get obj error|obj_group[%d] obj_index[%d] is not used", __FUNCTION__, obj_group->group_id, obj_id.index);
		return NULL;
	}

	*/
	if (obj_group->obj_index_[obj_id.index].obj_id.id != obj_id.id) {
		LOG_ERROR("get obj error|obj_group[%d] obj_id[%zu, %zu] mismatch", 
			obj_group->group_id, obj_group->obj_index_[obj_id.index].obj_id.id, obj_id.id);
		return NULL;
	}
	
	return (void *)(obj_group->obj_node_ + obj_id.index * obj_group->max_class_size);
	
}

void ShmPool::FreeObject(ObjGroup* obj_group, const ObjId & obj_id) {
	if (obj_group->used_obj_count <= 0 || obj_group->used_index_head == -1) {
		LOG_ERROR("obj_group not used|group_id[%d] used_obj_count[%d] used_index_head[%d]",
			obj_group->group_id, obj_group->used_obj_count, obj_group->used_index_head);
		return;
	}

	/// 先检查obj_id是否匹配 
	if (obj_group->obj_index_[obj_id.index].obj_id.id != obj_id.id) {
		LOG_ERROR("get obj error|obj_group[%d] obj_id[%zu, %zu] mismatch", 
			obj_group->group_id, obj_group->obj_index_[obj_id.index].obj_id.id, obj_id.id);
		return;
	}

	/// 从使用链表中查找
	int index = obj_group->used_index_head;
	int prev = index;
	while (index != -1) {
		if (index == obj_id.index) {
			break;
		}
		prev = index;
		index = obj_group->obj_index_[index].next;
	}
	
	if (index != obj_id.index) {
		LOG_ERROR("get obj error|obj_group[%d] obj_index[%d] is not used", obj_group->group_id, obj_id.index);
		return;
	}

	if (prev == index) {
		obj_group->used_index_head = obj_group->obj_index_[index].next;
	} else {
		obj_group->obj_index_[prev].next = obj_group->obj_index_[index].next;
	}
		
	obj_group->used_obj_count--;
	obj_group->obj_index_[index].next = obj_group->free_index_head; 
	obj_group->obj_index_[index].obj_id.id = 0;
	obj_group->free_index_head = index;

	/// NOTE:一定要放在函数最后，防止析构函数再次进入该函数(函数可重入问题)
	void *obj = obj_group->obj_node_ + index * obj_group->max_class_size;
	ObjectCreatorMgr::Instance().FreeObject(obj, obj_id.class_id);
}

std::string ShmPool::DumpObjGroup(ObjGroup * obj_group) {
	std::ostringstream stream;
	stream << "group_id: " << obj_group->group_id << ";";
	stream << "max_obj_count:" << obj_group->max_obj_count << ";";
	stream << "used_obj_count:" << obj_group->used_obj_count << ";";
	stream << "used_index_head:";
	int index = obj_group->used_index_head;
	while(index != -1 ) {
		stream << "->[obj_id.index:" << index << ";";
		stream << "obj_id.seq:" << obj_group->obj_index_[index].obj_id.seq << "]";
		index = obj_group->obj_index_[index].next;
	} 
	stream << "\n";

	return stream.str();
}

std::string ShmPool::DumpObj() {
	std::ostringstream stream;

	for (int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
		stream << DumpObjGroup(shm_pool_header_->group_info.group[i].obj_group);
	}
	return stream.str();
}

} // namespace