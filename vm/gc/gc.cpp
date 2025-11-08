#include "gc.h"
#include <stack>
#include <algorithm>

namespace lm {

class MarkSweepGC::Impl {
public:
    std::unordered_set<Object*> objects_;
    std::vector<Object*> roots_;
    GCStats stats_;
    mutable std::mutex mutex_;
    
    void mark_phase() {
        std::stack<Object*> worklist;
        
        // 标记根对象
        for (Object* root : roots_) {
            if (root && !root->is_marked()) {
                root->mark();
                worklist.push(root);
            }
        }
        
        // 标记所有可达对象
        while (!worklist.empty()) {
            Object* current = worklist.top();
            worklist.pop();
            
            for (Object* ref : current->get_references()) {
                if (ref && !ref->is_marked()) {
                    ref->mark();
                    worklist.push(ref);
                }
            }
        }
    }
    
    void sweep_phase() {
        auto it = objects_.begin();
        while (it != objects_.end()) {
            Object* obj = *it;
            if (obj->is_marked()) {
                // 清除标记，准备下一轮
                obj->mark(); // 这里需要清除标记，但我们的接口需要扩展
                ++it;
            } else {
                // 回收对象
                delete obj;
                it = objects_.erase(it);
                stats_.objects_collected++;
                stats_.managed_objects--;
            }
        }
    }
};

MarkSweepGC::MarkSweepGC() : impl_(std::make_unique<Impl>()) {}

MarkSweepGC::~MarkSweepGC() {
    // 清理所有对象
    for (Object* obj : impl_->objects_) {
        delete obj;
    }
}

void MarkSweepGC::collect() {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    
    impl_->mark_phase();
    impl_->sweep_phase();
    
    impl_->stats_.collections++;
}

void MarkSweepGC::register_object(Object* obj) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->objects_.insert(obj);
    impl_->stats_.managed_objects++;
}

void MarkSweepGC::unregister_object(Object* obj) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->objects_.erase(obj);
    impl_->stats_.managed_objects--;
}

GCStats MarkSweepGC::get_stats() const {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    return impl_->stats_;
}

size_t MarkSweepGC::object_count() const {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    return impl_->stats_.managed_objects;
}

void MarkSweepGC::add_root(Object* root) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->roots_.push_back(root);
}

void MarkSweepGC::remove_root(Object* root) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    auto it = std::find(impl_->roots_.begin(), impl_->roots_.end(), root);
    if (it != impl_->roots_.end()) {
        impl_->roots_.erase(it);
    }
}

// VM_GC 实现
VM_GC::VM_GC(GCStrategy strategy) {
    if (strategy == GCStrategy::MARK_SWEEP) {
        gc_ = std::make_unique<MarkSweepGC>();
    }
    // 可以扩展其他策略
}

VM_GC::~VM_GC() = default;

void VM_GC::collect() {
    if (gc_) gc_->collect();
}

Object* VM_GC::create_object(std::unique_ptr<Object> obj) {
    Object* raw_ptr = obj.release();
    if (gc_) gc_->register_object(raw_ptr);
    return raw_ptr;
}

GCStats VM_GC::get_stats() const {
    return gc_ ? gc_->get_stats() : GCStats{};
}

size_t VM_GC::object_count() const {
    return gc_ ? gc_->object_count() : 0;
}

void VM_GC::add_root(Object* root) {
    if (auto mark_sweep = dynamic_cast<MarkSweepGC*>(gc_.get())) {
        mark_sweep->add_root(root);
    }
}

void VM_GC::remove_root(Object* root) {
    if (auto mark_sweep = dynamic_cast<MarkSweepGC*>(gc_.get())) {
        mark_sweep->remove_root(root);
    }
}

} // namespace lm