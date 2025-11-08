#pragma once
#include "../heap_manager.h"
#include "gc.h"
#include <vector>

namespace lm {

    /**
     * @brief 集成垃圾回收的堆管理器
     */
    class GCHeapManager {
    private:
        HeapManager heap_manager_;
        VM_GC gc_;
        std::vector<Object*> gc_roots_;  // GC根对象

    public:
        GCHeapManager(GCStrategy strategy = GCStrategy::MARK_SWEEP);
        ~GCHeapManager() = default;

        // 禁止拷贝
        GCHeapManager(const GCHeapManager&) = delete;
        GCHeapManager& operator=(const GCHeapManager&) = delete;

        // 对象管理接口
        uint64_t allocate(std::unique_ptr<Object> obj);
        Object* loadObject(uint64_t address) const;
        bool deallocate(uint64_t address);

        // 根对象管理（用于标记清除算法）
        void add_root(Object* root);
        void remove_root(Object* root);
        void clear_roots();

        // 垃圾回收
        void collect_garbage();
        GCStats get_gc_stats() const;

        // 便捷方法
        uint64_t create_integer(int64_t value);
        uint64_t create_boolean(bool value);
        uint64_t create_float(double value);
        uint64_t create_string(const std::string& value);
        uint64_t create_null();
        uint64_t create_array();
        uint64_t create_dict();

        // 查询接口
        size_t object_count() const;
        bool is_valid_address(uint64_t address) const;
        std::string debug_info() const;

        // 获取底层管理器（用于特殊操作）
        HeapManager& get_heap_manager() { return heap_manager_; }
        const HeapManager& get_heap_manager() const { return heap_manager_; }
    };

} // namespace lm