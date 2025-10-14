#ifndef RANDOM_REPLACEMENT_H
#define RANDOM_REPLACEMENT_H

#include "../Cache.h"
#include <unordered_map>
#include <vector>
#include <random>

class RandomReplacement : public Cache {
private:
    std::unordered_map<std::string, std::string> data;
    std::vector<std::string> keys;
    std::mt19937 rng;
    
    void evictRandom();
    
public:
    RandomReplacement(int capacity);
    ~RandomReplacement() override;
    
    std::optional<std::string> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
    bool contains(const std::string& key) const override;
    void clear() override;
    std::string getType() const override { return "RandomReplacement"; }
};

#endif