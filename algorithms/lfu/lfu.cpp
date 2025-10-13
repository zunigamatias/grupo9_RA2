#include "Lfu.h"

Lfu::Lfu(int capacity) : Cache(capacity), minFrequency(0) {}

Lfu::~Lfu() {
    clear();
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