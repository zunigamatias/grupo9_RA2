#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include "../algorithms/Cache.h"

class Simulator {
private:
    std::vector<std::string> cacheTypes;
    int testCapacity;
    std::vector<int> testSequence;
    
public:
    Simulator();
    ~Simulator();
    
    // Main simulation function
    std::string runSimulation();
    
private:
    // Helper functions
    void generateTestSequence();
    double testCacheAlgorithm(const std::string& algorithmName);
    void saveWinnerToFile(const std::string& winner);
    std::unique_ptr<Cache> createTestCache(const std::string& cacheType);
};

// Global function for main.cpp
void runSimulationMode();

#endif // SIMULATOR_H