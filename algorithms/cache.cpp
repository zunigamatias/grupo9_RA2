#include <stdexcept>
#include "Cache.h"


Cache::Cache(int cap) : capacity(cap), currentSize(0) {
    if (cap <= 0) {
        throw std::invalid_argument("Cache capacity must be positive");
    }
}

Cache::~Cache()
{
}