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
