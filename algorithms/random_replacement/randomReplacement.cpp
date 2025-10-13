#include "RandomReplacement.h"

RandomReplacement::RandomReplacement(int capacity) : Cache(capacity) {
    // Initialize your Random Replacement-specific data structures here
}

RandomReplacement::~RandomReplacement() {
    clear();
}

std::optional<std::string> RandomReplacement::get(const std::string& key) {
    // TODO: Implement Random Replacement get logic
    return std::nullopt;
}

void RandomReplacement::put(const std::string& key, const std::string& value) {
    // TODO: Implement Random Replacement put logic
}

bool RandomReplacement::contains(const std::string& key) const {
    // TODO: Implement contains logic
    return false;
}

void RandomReplacement::clear() {
    // TODO: Clear all data structures
    currentSize = 0;
}