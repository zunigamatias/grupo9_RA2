#ifndef LFU_H
#define LFU_H

#include "../Cache.h"
#include <unordered_map>
#include <list>

class Lfu : public Cache {
private:
    // private data
    
public:
    Lfu(int capacity);
    ~Lfu() override;
    
    std::optional<std::string> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
    bool contains(const std::string& key) const override;
    void clear() override;
    std::string getType() const override { return "LFU"; }
};

#endif // LFU_H