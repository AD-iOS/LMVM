#pragma once
#include "models/models.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstdint>

namespace lm {

/**
 * @brief 堆管理器 - 集中管理所有对象指针
 */
class HeapManager {
private:
    // 对象存储
    std::vector<std::unique_ptr<Object>> objects_;
    std::unordered_map<uint64_t, Object*> address_map_;  // 地址到对象的映射
    
    // 空闲地址管理
    std::vector<uint64_t> free_addresses_;
    uint64_t next_address_ = 1;  // 0 保留为无效地址
    
    // 同步控制
    mutable std::mutex mutex_;
    
    // 分配新地址
    uint64_t allocate_address();
    
    // 释放地址
    void free_address(uint64_t address);

public:
    HeapManager();
    ~HeapManager() = default;
    
    // 禁止拷贝
    HeapManager(const HeapManager&) = delete;
    HeapManager& operator=(const HeapManager&) = delete;
    
    // 基本对象操作
    uint64_t store_object(std::unique_ptr<Object> obj);
    Object* load_object(uint64_t address) const;
    bool free_object(uint64_t address);
    
    // 批量操作
    std::vector<uint64_t> store_objects(const std::vector<std::unique_ptr<Object>>& objects);
    std::vector<Object*> load_objects(const std::vector<uint64_t>& addresses) const;
    
    // 类型特定的便捷方法
    uint64_t store_integer(int64_t value);
    uint64_t store_boolean(bool value);
    uint64_t store_float(double value);
    uint64_t store_string(const std::string& value);
    uint64_t store_null();
    
    // 查询和统计
    size_t object_count() const;
    size_t free_address_count() const;
    bool address_valid(uint64_t address) const;
    std::vector<uint64_t> get_all_addresses() const;
    
    // 内存管理
    void compact();  // 内存压缩，整理空闲空间
    void clear();    // 清空所有对象
    
    // 调试信息
    std::string debug_info() const;
    
    // 迭代器支持（用于GC遍历）
    class Iterator {
    private:
        const HeapManager& manager_;
        size_t index_;
        
    public:
        Iterator(const HeapManager& manager, size_t index);
        
        Object* operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
    };
    
    Iterator begin() const;
    Iterator end() const;
};

} // namespace lm