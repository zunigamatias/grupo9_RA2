#include "S3Fifo.h"

S3Fifo::S3Fifo(int capacity) : Cache(capacity) {
}

S3Fifo::~S3Fifo() {
    clear();
}

std::optional<std::string> S3Fifo::get(const std::string& key) {
    auto it = keyToNode.find(key);
    if (it == keyToNode.end()) {
        return std::nullopt;
    }
    
    it->second->visited = true;
    return it->second->value;
}

void S3Fifo::put(const std::string& key, const std::string& value) {
    if (capacity <= 0) return;
    
    auto it = keyToNode.find(key);
    if (it != keyToNode.end()) {
        it->second->value = value;
        it->second->visited = true;
        return;
    }
    
    if (currentSize >= capacity) {
        evictS3Fifo();
    }
    
    fifoQueue.emplace_back(key, value);
    keyToNode[key] = std::prev(fifoQueue.end());
    currentSize++;
}

bool S3Fifo::contains(const std::string& key) const {
    return keyToNode.find(key) != keyToNode.end();
}

void S3Fifo::clear() {
    keyToNode.clear();
    fifoQueue.clear();
    currentSize = 0;
}

void S3Fifo::evictS3Fifo() {
    if (fifoQueue.empty()) return;
    
    auto it = fifoQueue.begin();
    
    while (it != fifoQueue.end()) {
        if (!it->visited) {
            std::string keyToRemove = it->key;
            keyToNode.erase(keyToRemove);
            fifoQueue.erase(it);
            currentSize--;
            return;
        } else {
            it->visited = false;
            ++it;
        }
    }
    
    if (!fifoQueue.empty()) {
        std::string keyToRemove = fifoQueue.front().key;
        keyToNode.erase(keyToRemove);
        fifoQueue.pop_front();
        currentSize--;
    }
}
