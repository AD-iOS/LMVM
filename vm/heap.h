#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include "models/models.h"

class HeapManager {
private:
    std::vector<LmObject*> heap;  // 堆存储
    std::unordered_map<size_t, bool> freeList;    // 空闲列表
    size_t heapSize;                              // 堆大小
    size_t usedSize;                              // 已使用大小
    
public:
    explicit HeapManager(size_t initialSize = 256);  // 初始堆大小
    ~HeapManager();
    
    // 分配内存
    size_t allocate(size_t size);
    
    // 释放内存
    void deallocate(size_t address);
    
    // 存储对象到堆
    size_t storeObject(LmObject* obj);
    
    // 从堆加载对象
    LmObject* loadObject(size_t address);

    // 更改对象
    void updateObject(size_t address, void *obj);
    
    // 垃圾回收（简单标记）
    void garbageCollect();
    
    // 堆信息
    size_t getHeapSize() const { return heapSize; }
    size_t getUsedSize() const { return usedSize; }
    double getUsageRatio() const { return static_cast<double>(usedSize) / heapSize; }
};