#include "Lfu.h"

Lfu::Lfu(int capacity) : Cache(capacity) {
    // Initialize your LFU-specific data structures here
}

Lfu::~Lfu() {
    clear();
}

std::optional<std::string> Lfu::get(const std::string& key) {
    // TODO: Implement LFU get logic
    return std::nullopt;
}

void Lfu::put(const std::string& key, const std::string& value) {
    // TODO: Implement LFU put logic
}

bool Lfu::contains(const std::string& key) const {
    // TODO: Implement contains logic
    return false;
}

void Lfu::clear() {
    // TODO: Clear all data structures
    currentSize = 0;
}