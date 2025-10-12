#ifndef CACHE_H
#define CACHE_H

#include <string>
#include <utility>
class Cache {
public:
    virtual std::pair<std::string, bool> getItem(int key) = 0;
    virtual int putItem(int key, std::string text);
    virtual ~Cache() = default;
};

#endif
