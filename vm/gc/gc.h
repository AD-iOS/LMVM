#pragma once
#include "../models/object.h"
#include <vector>
#include <unordered_set>
#include <memory>
#include <mutex>

namespace lm {

    /**
     * @brief 垃圾回收策略
     */
    enum class GCStrategy {
        REFERENCE_COUNTING,  // 引用计数
        MARK_SWEEP           // 标记清除
    };

    /**
     * @brief GC统计信息
     */
    struct GCStats {
        size_t collections = 0;
        size_t objects_collected = 0;
        size_t managed_objects = 0;
    };

    /**
     * @brief 垃圾回收器基类
     */
    class GarbageCollector {
    public:
        virtual ~GarbageCollector() = default;

        virtual void collect() = 0;
        virtual void register_object(Object* obj) = 0;
        virtual void unregister_object(Object* obj) = 0;
        [[nodiscard]] virtual GCStats get_stats() const = 0;
        [[nodiscard]] virtual size_t object_count() const = 0;
    };

    /**
     * @brief 标记清除垃圾回收器
     */
    class MarkSweepGC : public GarbageCollector {
    public:
        MarkSweepGC();
        ~MarkSweepGC() override;

        void collect() override;
        void register_object(Object* obj) override;
        void unregister_object(Object* obj) override;
        [[nodiscard]] GCStats get_stats() const override;
        [[nodiscard]] size_t object_count() const override;

        void add_root(Object* root);
        void remove_root(Object* root);

    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
    };

    /**
     * @brief 虚拟机垃圾回收管理器
     */
    class VM_GC {
    public:
        explicit VM_GC(GCStrategy strategy = GCStrategy::MARK_SWEEP);
        ~VM_GC();

        void collect();
        Object* create_object(std::unique_ptr<Object> obj);
        [[nodiscard]] GCStats get_stats() const;
        [[nodiscard]] size_t object_count() const;

        void add_root(Object* root);
        void remove_root(Object* root);

    private:
        std::unique_ptr<GarbageCollector> gc_;
    };

} // namespace lm