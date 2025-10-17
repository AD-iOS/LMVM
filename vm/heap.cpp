#include "heap.h"
#include "error/error.h"


HeapManager::HeapManager(size_t initialSize) : heapSize(initialSize), usedSize(0) {
    heap.resize(heapSize);  // 初始化空闲列表
    freeList[0] = true;             // 整个堆初始为空闲
}

HeapManager::~HeapManager() {
    for (const auto& c: heap) {
        delete c;
    }
}

size_t HeapManager::allocate(size_t size) {
    for (const auto& [start, isFree] : freeList) {
        if (isFree) {// 是否有足够的连续空间
            
            size_t end = start + size;
            if (end <= heapSize) {
                freeList[start] = false;  // 标记为已使用
                usedSize += size;
                return start;
            }
        }
    }
    
    // 没有足够空间，进行垃圾回收
    garbageCollect();
    
    // 再次尝试分配
    for (const auto& [start, isFree] : freeList) {
        if (isFree) {
            size_t end = start + size;
            if (end <= heapSize) {
                freeList[start] = false;
                usedSize += size;
                return start;
            }
        }
    }
    
    throw LmError("Not enough memory");
}

void HeapManager::deallocate(size_t address) {
    if (address >= heapSize) {
        throw LmError("Invalid heap address");
    }
    
    if (freeList.contains(address) && !freeList[address]) {
        freeList[address] = true;  // 标记为空闲
        delete heap[address];
        usedSize--;
    }
}

size_t HeapManager::storeObject(LmObject* obj) {
    size_t address = allocate(1);  // 每个对象占用一个槽位
    heap[address] = obj;
    return address;
}

LmObject* HeapManager::loadObject(size_t address) {
    if (address >= heapSize || !freeList.contains(address) || freeList[address]) {
        throw LmError("Invalid heap address or address is free");
    }
    return heap[address];
}

void HeapManager::updateObject(size_t address, void* obj) {
    if (address >= heapSize || !freeList.contains(address) || freeList[address]) {
        throw LmError("Invalid heap address or address is free");
    }
    heap[address]->update_value(obj);
}

void HeapManager::garbageCollect() {
    for (auto& [address, isFree] : freeList) {
        if (!isFree && !heap[address]) {
            isFree = true;  // 对象已被释放，标记为空闲
            usedSize--;
        }
    }
}