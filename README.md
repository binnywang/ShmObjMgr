# ShmObjMgr 共享内存对象管理器
## 主要特性
- 一种通用的对象池实现
- 支持存放在放在共享内存中，支持系统重启后热加载，支持对象中有原始指针，虚函数，重启会进行恢复。
- 可选的智能指针封装，实现参考boost/shared_ptr， 智能指针计数器和智能指针本身也支持存放在共享内存上（**如果对象池和智能指针存放在共享内存中，在使用上有一定的限制，如果在栈上使用智能指针出现core，智能指针析构函数没有被调用，进程重启时，出现指针所指的类对象或者指针计数器对象没有正确释放，会造成内存泄漏**）
## 设计思路
- 首先给类分配uint8_t的class_id,在分配和释放对象时，需要通过class_id来调用类的构造和析构函数。
- 对类进行group,  采用uint8_t 的group_id来标示，同一个group中的类对象共享相同的单个内存块大小和内存块个数
- 注册类到group中， 注册时需要提供{class_id, class_size, class_obj_count}三元组，如分别注册{class_a_id, class_a_size, class_a_obj_count}, {class_b_id, class_b_size, class_b_obj_count}, 最终group占用总的内存大小为 max(class_a_size, class_b_size) * max(max_a_obj_count, max_b_obj_count)

- 每个对象由一个uint64_t的obj_id标示，由group_id, class_id, index, seq四部分组成。如果不使用智能指针，每次访问对象指针obj_ptr时，必须通过obj_id去查找对应的obj_ptr, 防止野指针访问。

- 每个类必须继承ShmObj类， 有个Recover接口，在热加载的时候会被调用，放在共享内存的类成员变量里面有原始指针，可以在该接口里面对原始指针进行重置。

## FIXME
- 为了通过obj_id快速查询到obj_ptr, class_id定义为uint8_t, 直接通过数组下标遍历， 目前采用枚举的方法，需要修改公共头文件头obj_creator_mgr.h。
- 如果使用智能指针，智能指针的计数器个数该分配多少个？

## Example
- 不使用智能指针



- 使用智能指针