#include "shm_pool.h"

#include <time.h>

ShmPool::ShmPool() {
	id_generator_ = time(NULL);
}


int ShmPool::NewClassGroup(GRPUPID & group_id) {
	if (tmp_group_info_.group_num >= MAX_CLASS_GROUP_NUM) {
		return -1;		
	}
	
	group_id = GeneratorNewID();
	tmp_group_info_.group[tmp_group_info_.group_num].group_id = group_id;
	tmp_group_info_.group_num++;
	return 0;
}

int ShmPool::AddClassToGroup(GRPUPID & group_id,int class_id,size_t class_size,size_t obj_count) {
	int i = 0;
	for ( ; i < tmp_group_info_.group_num; i++) {
		if (group_id == tmp_group_info_.group[i].group_id) {
			break;
		}
	}

	if (i >= tmp_group_info_.group_num) {
		return -1;
	}

	if (tmp_group_info_.group[i].class_id_num >= MAX_CLASS_PER_GROUP) {
		return -2;
	}

	tmp_group_info_.group[i].class_id[tmp_group_info_.group[i].class_id_num] = class_id;
	tmp_group_info_.group[i].class_id_num++;
	tmp_group_info_.group[i].max_obj_count = tmp_group_info_.group[i].max_obj_count >= obj_count ? tmp_group_info_.group[i].max_obj_count : obj_count;
	tmp_group_info_.group[i].max_class_size = tmp_group_info_.group[i].max_class_size >= class_size ? tmp_group_info_.group[i].max_class_size : class_size;
	return 0;
}


size_t ShmPool::TotalSize() {
	size_t total_size = 0;
	for (int i = 0; i < tmp_group_info_.group_num; i++) {
		total_size += sizeof(ObjGroup) + tmp_group_info_.group[i].max_obj_count * (sizeof(ObjIndex) + tmp_group_info_.group[i].max_class_size);
	}

	total_size += sizeof(ShmPoolHeader);
	return total_size;
}


int ShmPool::Init(void * mem,size_t max_size,bool fresh) {
	if (mem == NULL || max_size < TotalSize()) {
		return -1;
	}

	if (fresh) {
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
			for (int j = 0; j < shm_pool_header_->group_info.group[i].class_id_num; i++) {
 				obj_group->class_id[j] = shm_pool_header_->group_info.group[i].class_id[j];
 			}
			shm_pool_header_->group_info.group[i].obj_group = obj_group;

 			InitObjGroup(obj_group);
			obj_group = reinterpret_cast<ObjGroup*>((char*)obj_group + obj_group->group_size);
 		}
	} else {
		/// recovery
		
	}
}


void ShmPool::InitObjGroup(ObjGroup * obj_group) {
	obj_group->obj_index_ = reinterpret_cast<ObjIndex*>((char*)obj_group + sizeof(ObjGroup));
	obj_group->obj_node_ = reinterpret_cast<ObjNode*>((char*)obj_group->obj_index_ + sizeof(ObjIndex) * obj_group->max_obj_count);
	obj_group->used_index_head = -1;
	obj_group->free_index_head = 0;

	int i = 0;
	for( ; i < obj_group->max_obj_count - 1; i++) {
		///ObjIndex[i].index = i;
		obj_group->obj_index_[i].next = i + 1;
	}
	
	obj_group->obj_index_[i].next = -1;
}

ShmPool::ObjGroup* ShmPool::FindGroupByClassId(int class_id) {
	for (int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
		for(int j = 0; j < shm_pool_header_->group_info.group[i].class_id_num; j++) {
			if (shm_pool_header_->group_info.group[i].class_id[j] == class_id) {
				return shm_pool_header_->group_info.group[i].obj_group;
			}
		}
	}
	return NULL;
}

ShmPool::ObjGroup* ShmPool::FindGroupByGroupId(int group_id) {
	for (int i = 0; i < shm_pool_header_->group_info.group_num; i++) {
		if (shm_pool_header_->group_info.group[i].group_id == group_id) {
			return shm_pool_header_->group_info.group[i].obj_group;
		}
	}
	
	return NULL;
}

void* ShmPool::NewObject(int class_id,ObjID & obj_id) {
	ObjGroup* obj_group = FindGroupByClassId(class_id);
	if (obj_group == NULL) {
		return NULL;
	}
	
	return NewObject(obj_group, class_id, obj_id);
}

void* ShmPool::GetObject(const ObjID & obj_id) {
	ObjGroup* obj_group = FindGroupByGroupId(obj_id.group_id);
	if (obj_group == NULL) {
		return NULL;
	}
	
	return GetObject(obj_group, obj_id);
}


void ShmPool::FreeObject(const ObjID & obj_id) {
	ObjGroup* obj_group = FindGroupByGroupId(obj_id.group_id);
	if (obj_group == NULL) {
		return;
	}
	FreeObject(obj_group, obj_id);
}

void* ShmPool::NewObject(ObjGroup* obj_group, int class_id, ObjID & obj_id) {
	if (obj_group->free_index_head == -1 || obj_group->used_obj_count >= obj_group->max_obj_count) {
		return NULL;
	}
	
	int index = obj_group->free_index_head;
	obj_group->free_index_head = obj_group->obj_index_[index].next;
	
	obj_group->obj_index_[index].next = obj_group->used_index_head;
	obj_group->used_index_head = index;
	
	obj_group->used_obj_count++;
	obj_group->obj_index_[index].class_id = class_id;
	/// void* obj = creator_array[class_id]->PlacementNew((void*)obj_group->obj_node_[index].data);
	obj_id.group_id = obj_group->group_id;
	obj_id.obj_index = index;
	
	return (void *)(obj_group->obj_node_ + index * sizeof(obj_group->max_class_size));
}

void* ShmPool::GetObject(ObjGroup* obj_group, const ObjID & obj_id) {
	///uint32_t obj_index = obj_id.obj_index;
	if (obj_group->used_obj_count <= 0 || obj_group->used_index_head == -1) {
		return NULL;
	}
	int index = obj_group->used_index_head;
	while (index != -1) {
		if (index == obj_id.obj_index) {
			break;
		}
		index = obj_group->obj_index_[index].next;
	}
	
	if (index != obj_id.obj_index) {
		return NULL;
	}
	
	return (void *)(obj_group->obj_node_ + index * sizeof(obj_group->max_class_size));
	
}

void ShmPool::FreeObject(ObjGroup* obj_group, const ObjID & obj_id) {
	if (obj_group->used_obj_count <= 0 || obj_group->used_index_head == -1) {
		return;
	}
	
	int index = obj_group->used_index_head;
	while (index != -1) {
		if (index == obj_id.obj_index) {
			break;
		}
		index = obj_group->obj_index_[index].next;
	}
	
	if (index != obj_id.obj_index) {
		return;
	}
	/// memset(obj_group->obj_node_ + index * sizeof(obj_group->max_class_size, 0, sizeof(objGroup->max_class_size));
	obj_group->obj_index_[obj_id.obj_index].next =  obj_group->used_index_head;
	obj_group->used_index_head = obj_id.obj_index;
	obj_group->used_obj_count--;
}