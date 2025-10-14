#ifndef S3_FIFO_H
#define S3_FIFO_H

#include "../Cache.h"
#include <unordered_map>
#include <deque>

struct S3FifoNode {
    std::string key;
    std::string value;
    bool visited;
    
    S3FifoNode(const std::string& k, const std::string& v) 
        : key(k), value(v), visited(false) {}
};

class S3Fifo : public Cache {
private:
    std::unordered_map<std::string, std::deque<S3FifoNode>::iterator> keyToNode;
    std::deque<S3FifoNode> fifoQueue;
    
    void evictS3Fifo();
    
public:
    S3Fifo(int capacity);
    ~S3Fifo() override;
    
    std::optional<std::string> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
    bool contains(const std::string& key) const override;
    void clear() override;
    std::string getType() const override { return "S3Fifo"; }
};

#endif