#ifndef S3_FIFO_H
#define S3_FIFO_H

#include "../Cache.h"
#include <unordered_map>
#include <queue>

class S3Fifo : public Cache {
private:
    // Add your data structures here for S3-FIFO implementation
    
public:
    S3Fifo(int capacity);
    ~S3Fifo() override;
    
    std::optional<std::string> get(const std::string& key) override;
    void put(const std::string& key, const std::string& value) override;
    bool contains(const std::string& key) const override;
    void clear() override;
    std::string getType() const override { return "S3Fifo"; }
};

#endif // S3_FIFO_H