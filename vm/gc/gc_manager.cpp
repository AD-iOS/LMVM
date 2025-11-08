#include "gc_manager.h"

#include <sstream>

#include "../models/array.h"

namespace lm {

GCHeapManager::GCHeapManager(const GCStrategy strategy) : gc_(strategy) {}

uint64_t GCHeapManager::allocate(std::unique_ptr<Object> obj) {
    if (!obj) {
        return 0;
    }
    
    uint64_t address = heap_manager_.store_object(std::move(obj));
    if (address != 0) {
        // 注册到GC系统
        Object* raw_obj = heap_manager_.load_object(address);
        if (raw_obj) {
            gc_.add_root(raw_obj);  // 暂时作为根对象，实际使用时需要更精确的管理
        }
    }
    
    return address;
}

Object* GCHeapManager::loadObject(uint64_t address) const {
    return heap_manager_.load_object(address);
}

bool GCHeapManager::deallocate(uint64_t address) {
    Object* obj = heap_manager_.load_object(address);
    if (obj) {
        // 从GC根对象中移除
        remove_root(obj);
    }
    return heap_manager_.free_object(address);
}

void GCHeapManager::add_root(Object* root) {
    if (root) {
        gc_roots_.push_back(root);
    }
}

void GCHeapManager::remove_root(Object* root) {
    const auto it = std::ranges::find(gc_roots_.begin(), gc_roots_.end(), root);
    if (it != gc_roots_.end()) {
        gc_roots_.erase(it);
    }
}

void GCHeapManager::clear_roots() {
    gc_roots_.clear();
}

void GCHeapManager::collect_garbage() {
    gc_.collect();
}

GCStats GCHeapManager::get_gc_stats() const {
    return gc_.get_stats();
}

uint64_t GCHeapManager::create_integer(int64_t value) {
    return allocate(std::make_unique<Integer>(value));
}

uint64_t GCHeapManager::create_boolean(bool value) {
    return allocate(std::make_unique<Boolean>(value));
}

uint64_t GCHeapManager::create_float(double value) {
    return allocate(std::make_unique<Float>(value));
}

uint64_t GCHeapManager::create_string(const std::string& value) {
    return allocate(std::make_unique<String>(value));
}

uint64_t GCHeapManager::create_null() {
    return allocate(std::make_unique<NullObject>());
}

uint64_t GCHeapManager::create_array() {
    return allocate(std::make_unique<Array>());
}

size_t GCHeapManager::object_count() const {
    return heap_manager_.object_count();
}

bool GCHeapManager::is_valid_address(uint64_t address) const {
    return heap_manager_.address_valid(address);
}

std::string GCHeapManager::debug_info() const {
    std::ostringstream oss;
    oss << heap_manager_.debug_info() << ", " << gc_.get_stats().managed_objects << " managed by GC";
    return oss.str();
}

} // namespace lm