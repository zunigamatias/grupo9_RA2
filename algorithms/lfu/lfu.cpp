#include "Lfu.h"

Lfu::Lfu(int capacity) : Cache(capacity), minFrequency(0) {}

Lfu::~Lfu() {
    clear();
}

std::optional<std::string> Lfu::get(const std::string& key) {
    auto it = keyToNode.find(key);
    if (it == keyToNode.end()) {
        return std::nullopt;
    }
    
    std::string value = it->second->value;
    updateFrequency(key);
    return value;
}

void Lfu::put(const std::string& key, const std::string& value) {
    if (capacity <= 0) return;
    
    auto it = keyToNode.find(key);
    if (it != keyToNode.end()) {
        it->second->value = value;
        updateFrequency(key);
        return;
    }
    
    if (currentSize >= capacity) {
        evictLfu();
    }
    
    frequencyGroups[1].emplace_front(key, value);
    keyToNode[key] = frequencyGroups[1].begin();
    minFrequency = 1;
    currentSize++;
}

bool Lfu::contains(const std::string& key) const {
    return keyToNode.find(key) != keyToNode.end();
}

void Lfu::clear() {
    keyToNode.clear();
    frequencyGroups.clear();
    currentSize = 0;
    minFrequency = 0;
}

void Lfu::updateFrequency(const std::string& key) {
    auto nodeIt = keyToNode[key];
    int oldFreq = nodeIt->frequency;
    int newFreq = oldFreq + 1;
    
    LfuNode newNode = *nodeIt;
    newNode.frequency = newFreq;
    
    frequencyGroups[oldFreq].erase(nodeIt);
    
    frequencyGroups[newFreq].emplace_front(newNode);
    keyToNode[key] = frequencyGroups[newFreq].begin();
    
    if (frequencyGroups[minFrequency].empty()) {
        minFrequency++;
    }
}

void Lfu::evictLfu() {
    if (frequencyGroups[minFrequency].empty()) return;
    
    auto& minFreqList = frequencyGroups[minFrequency];
    std::string keyToRemove = minFreqList.back().key;
    
    minFreqList.pop_back();
    keyToNode.erase(keyToRemove);
    currentSize--;
}