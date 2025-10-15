#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <random>
#include "../algorithms/Cache.h"

struct SimulationResult {
    std::string algorithmName;
    std::string randomType;
    int userId;
    int totalRequests;
    int cacheHits;
    int cacheMisses;
    double totalTime;
    double averageTime;
    std::vector<int> missedFiles;
    std::vector<double> accessTimes;
};

class Simulator {
private:
    std::vector<std::string> cacheTypes;
    int testCapacity;
    int numUsers;
    int requestsPerTest;
    std::mt19937 rng;
    
    std::uniform_int_distribution<int> uniformDist;
    std::poisson_distribution<int> poissonDist;
    
public:
    Simulator();
    ~Simulator();
    
    std::string runSimulation();
    
private:
    std::vector<int> generateRandomSequence(int count);
    std::vector<int> generatePoissonSequence(int count);
    std::vector<int> generateBiasedSequence(int count);
    
    SimulationResult testCacheAlgorithm(const std::string& algorithmName, 
                                      const std::string& randomType,
                                      int userId,
                                      const std::vector<int>& sequence);
    
    std::unique_ptr<Cache> createTestCache(const std::string& cacheType);
    void saveWinnerToFile(const std::string& winner);
    void printSimulationResults(const std::vector<SimulationResult>& results);
    std::string selectBestAlgorithm(const std::vector<SimulationResult>& results);
    std::string readTestFile(int fileNumber);
};

void runSimulationMode();

#endif
