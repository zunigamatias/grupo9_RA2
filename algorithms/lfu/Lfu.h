#ifndef LFU_H
#define LFU_H

#include "../Cache.h"
#include <unordered_map>
#include <list>

struct LfuNode {
    std::string key;
    std::string value;
    int frequency;
    
    LfuNode(const std::string& k, const std::string& v) 
        : key(k), value(v), frequency(1) {}
};

class Lfu : public Cache {
private:
    std::unordered_map<std::string, std::list<LfuNode>::iterator> keyToNode;
    std::unordered_map<int, std::list<LfuNode>> frequencyGroups;
    int minFrequency;
    
    void updateFrequency(const std::string& key);
    void evictLfu();
    
public:
    Lfu(int capacity);
    ~Lfu() override;
    
    std::optional<std::string> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
    bool contains(const std::string& key) const override;
    void clear() override;
    std::string getType() const override { return "LFU"; }
};

#endif