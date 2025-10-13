#include "Simulator.h"
#include "../algorithms/lfu/Lfu.h"
#include "../algorithms/random_replacement/RandomReplacement.h"
#include "../algorithms/s3_fifo/S3Fifo.h"
#include "../core/disk_io/DiskIo.h"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

Simulator::Simulator() : testCapacity(50) {
    cacheTypes = {"lfu", "random", "s3_fifo"};
    generateTestSequence();
}

Simulator::~Simulator() {
    // Destructor
}

std::string Simulator::runSimulation() {
    std::cout << "Starting cache algorithm simulation...\n";
    
    std::string bestAlgorithm = "lfu";  
    double bestHitRate = 0.0;
    
    for (const std::string& algorithm : cacheTypes) {
        std::cout << "Testing " << algorithm << " algorithm...\n";
        
        double hitRate = testCacheAlgorithm(algorithm);
        std::cout << algorithm << " hit rate: " << hitRate * 100 << "%\n";
        
        if (hitRate > bestHitRate) {
            bestHitRate = hitRate;
            bestAlgorithm = algorithm;
        }
    }
    
    std::cout << "\nSimulation complete!\n";
    std::cout << "Winner: " << bestAlgorithm << " with " << bestHitRate * 100 << "% hit rate\n";
    
    saveWinnerToFile(bestAlgorithm);
    
    return bestAlgorithm;
}

void Simulator::generateTestSequence() {
    testSequence.clear();
    testSequence.reserve(1000);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    
    for (int i = 0; i < 1000; ++i) {
        testSequence.push_back(dis(gen));
    }
    
    std::cout << "Generated test sequence of " << testSequence.size() << " file accesses\n";
}

double Simulator::testCacheAlgorithm(const std::string& algorithmName) {
    auto cache = createTestCache(algorithmName);
    if (!cache) {
        std::cerr << "Failed to create cache for " << algorithmName << std::endl;
        return 0.0;
    }
    
    int hits = 0;
    int totalAccesses = testSequence.size();
    
    for (int fileNumber : testSequence) {
        std::string key = std::to_string(fileNumber);
        
        auto result = cache->get(key);
        if (result.has_value()) {
            // Cache hit
            hits++;
        } else {
            // Cache miss - simulate loading file and putting in cache
            std::string fakeFileContent = "Content of file " + std::to_string(fileNumber);
            cache->put(key, fakeFileContent);
        }
    }
    
    double hitRate = static_cast<double>(hits) / totalAccesses;
    return hitRate;
}

void Simulator::saveWinnerToFile(const std::string& winner) {
    std::ofstream file("winner_algorithm.txt");
    if (file.is_open()) {
        file << winner;
        file.close();
        std::cout << "Winner algorithm saved to winner_algorithm.txt\n";
    } else {
        std::cerr << "Error: Could not save winner to file\n";
    }
}

std::unique_ptr<Cache> Simulator::createTestCache(const std::string& cacheType) {
    if (cacheType == "lfu") {
        return std::make_unique<Lfu>(testCapacity);
    } else if (cacheType == "random") {
        return std::make_unique<RandomReplacement>(testCapacity);
    } else if (cacheType == "s3_fifo") {
        return std::make_unique<S3Fifo>(testCapacity);
    }
    
    return nullptr;
}

void runSimulationMode() {
    Simulator simulator;
    simulator.runSimulation();
}