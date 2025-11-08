#include "heap_manager.h"
#include "models/models.h"
#include <algorithm>
#include <sstream>

namespace lm {

HeapManager::HeapManager() {
    // 预分配一些地址空间
    free_addresses_.reserve(1024);
}

uint64_t HeapManager::allocate_address() {
    if (!free_addresses_.empty()) {
        uint64_t address = free_addresses_.back();
        free_addresses_.pop_back();
        return address;
    }
    return next_address_++;
}

void HeapManager::free_address(uint64_t address) {
    if (address > 0 && address < next_address_) {
        free_addresses_.push_back(address);
        // 保持空闲列表有序（可选）
        std::sort(free_addresses_.begin(), free_addresses_.end());
    }
}

uint64_t HeapManager::store_object(std::unique_ptr<Object> obj) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!obj) {
        return 0;  // 无效地址
    }
    
    uint64_t address = allocate_address();
    
    // 存储对象
    Object* raw_ptr = obj.get();
    objects_.push_back(std::move(obj));
    address_map_[address] = raw_ptr;
    
    return address;
}

Object* HeapManager::load_object(uint64_t address) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = address_map_.find(address);
    if (it != address_map_.end()) {
        return it->second;
    }
    return nullptr;
}

bool HeapManager::free_object(uint64_t address) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto it = address_map_.find(address);
    if (it != address_map_.end()) {
        // 从objects_中删除（通过查找指针）
        auto obj_it = std::find_if(objects_.begin(), objects_.end(),
            [it](const std::unique_ptr<Object>& obj) {
                return obj.get() == it->second;
            });
        
        if (obj_it != objects_.end()) {
            objects_.erase(obj_it);
        }
        
        address_map_.erase(it);
        free_address(address);
        return true;
    }
    return false;
}

std::vector<uint64_t> HeapManager::store_objects(const std::vector<std::unique_ptr<Object>>& objects) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<uint64_t> addresses;
    addresses.reserve(objects.size());
    
    for (const auto& obj : objects) {
        if (obj) {
            uint64_t address = allocate_address();
            Object* raw_ptr = obj.get();
            objects_.push_back(std::move(const_cast<std::unique_ptr<Object>&>(obj)));
            address_map_[address] = raw_ptr;
            addresses.push_back(address);
        } else {
            addresses.push_back(0);  // 无效地址
        }
    }
    
    return addresses;
}

std::vector<Object*> HeapManager::load_objects(const std::vector<uint64_t>& addresses) const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Object*> objects;
    objects.reserve(addresses.size());
    
    for (uint64_t addr : addresses) {
        auto it = address_map_.find(addr);
        objects.push_back(it != address_map_.end() ? it->second : nullptr);
    }
    
    return objects;
}

uint64_t HeapManager::store_integer(int64_t value) {
    return store_object(std::make_unique<Integer>(value));
}

uint64_t HeapManager::store_boolean(bool value) {
    return store_object(std::make_unique<Boolean>(value));
}

uint64_t HeapManager::store_float(double value) {
    return store_object(std::make_unique<Float>(value));
}

uint64_t HeapManager::store_string(const std::string& value) {
    return store_object(std::make_unique<String>(value));
}

uint64_t HeapManager::store_null() {
    return store_object(std::make_unique<NullObject>());
}

size_t HeapManager::object_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return objects_.size();
}

size_t HeapManager::free_address_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return free_addresses_.size();
}

bool HeapManager::address_valid(uint64_t address) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return address_map_.find(address) != address_map_.end();
}

std::vector<uint64_t> HeapManager::get_all_addresses() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<uint64_t> addresses;
    addresses.reserve(address_map_.size());
    
    for (const auto& pair : address_map_) {
        addresses.push_back(pair.first);
    }
    
    return addresses;
}

void HeapManager::compact() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // 简单的压缩：重新分配地址，消除碎片
    std::vector<std::unique_ptr<Object>> new_objects;
    std::unordered_map<uint64_t, Object*> new_address_map;
    
    new_objects.reserve(objects_.size());
    new_address_map.reserve(objects_.size());
    
    uint64_t new_address = 1;
    for (auto& obj : objects_) {
        if (obj) {
            new_objects.push_back(std::move(obj));
            new_address_map[new_address++] = new_objects.back().get();
        }
    }
    
    objects_ = std::move(new_objects);
    address_map_ = std::move(new_address_map);
    
    // 重置地址分配器
    free_addresses_.clear();
    next_address_ = new_address;
}

void HeapManager::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    objects_.clear();
    address_map_.clear();
    free_addresses_.clear();
    next_address_ = 1;
}

std::string HeapManager::debug_info() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::ostringstream oss;
    oss << "HeapManager["
        << "objects=" << objects_.size()
        << ", addresses=" << address_map_.size()
        << ", free_slots=" << free_addresses_.size()
        << ", next_address=" << next_address_
        << "]";
    
    return oss.str();
}

// 迭代器实现
HeapManager::Iterator::Iterator(const HeapManager& manager, size_t index)
    : manager_(manager), index_(index) {}

Object* HeapManager::Iterator::operator*() const {
    std::lock_guard<std::mutex> lock(manager_.mutex_);
    if (index_ < manager_.objects_.size()) {
        return manager_.objects_[index_].get();
    }
    return nullptr;
}

HeapManager::Iterator& HeapManager::Iterator::operator++() {
    ++index_;
    return *this;
}

bool HeapManager::Iterator::operator!=(const Iterator& other) const {
    return &manager_ != &other.manager_ || index_ != other.index_;
}

HeapManager::Iterator HeapManager::begin() const {
    return Iterator(*this, 0);
}

HeapManager::Iterator HeapManager::end() const {
    return Iterator(*this, objects_.size());
}

} // namespace lm