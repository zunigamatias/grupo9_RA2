#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <optional>
class Cache {
protected:
    int capacity;
    int currentSize;

public:
    Cache(int cap);
    virtual ~Cache();
    
    virtual std::optional<std::string> get(const std::string& key) = 0;
    virtual void put(const std::string& key, const std::string& value) = 0;
    virtual bool contains(const std::string& key) const = 0;
    virtual void clear() = 0;
    
    int getCapacity() const { return capacity; }
    int getCurrentSize() const { return currentSize; }
    bool isFull() const { return currentSize >= capacity; }
    bool isEmpty() const { return currentSize == 0; }
    
    virtual std::string getType() const = 0;
};

#endif
