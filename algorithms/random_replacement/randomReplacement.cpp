#include "RandomReplacement.h"
#include <algorithm>

RandomReplacement::RandomReplacement(int capacity) 
    : Cache(capacity), rng(std::random_device{}()) {
}

RandomReplacement::~RandomReplacement() {
    clear();
}

std::optional<std::string> RandomReplacement::get(const std::string& key) {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    return std::nullopt;
}

void RandomReplacement::put(const std::string& key, const std::string& value) {
    if (capacity <= 0) return;
    
    auto it = data.find(key);
    if (it != data.end()) {
        it->second = value;
        return;
    }
    
    if (currentSize >= capacity) {
        evictRandom();
    }
    
    data[key] = value;
    keys.push_back(key);
    currentSize++;
}

bool RandomReplacement::contains(const std::string& key) const {
    return data.find(key) != data.end();
}

void RandomReplacement::clear() {
    data.clear();
    keys.clear();
    currentSize = 0;
}

void RandomReplacement::evictRandom() {
    if (keys.empty()) return;
    
    std::uniform_int_distribution<size_t> dist(0, keys.size() - 1);
    size_t randomIndex = dist(rng);
    
    std::string keyToRemove = keys[randomIndex];
    data.erase(keyToRemove);
    
    keys[randomIndex] = keys.back();
    keys.pop_back();
    
    currentSize--;
}