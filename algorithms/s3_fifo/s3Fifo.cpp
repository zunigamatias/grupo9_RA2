#include "S3Fifo.h"

S3Fifo::S3Fifo(int capacity) : Cache(capacity) {
    // Initialize your S3-FIFO-specific data structures here
}

S3Fifo::~S3Fifo() {
    clear();
}

std::optional<std::string> S3Fifo::get(const std::string& key) {
    // TODO: Implement S3-FIFO get logic
    return std::nullopt;
}

void S3Fifo::put(const std::string& key, const std::string& value) {
    // TODO: Implement S3-FIFO put logic
}

bool S3Fifo::contains(const std::string& key) const {
    // TODO: Implement contains logic
    return false;
}

void S3Fifo::clear() {
    // TODO: Clear all data structures
    currentSize = 0;
}