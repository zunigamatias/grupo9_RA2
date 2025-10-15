#include "Simulator.h"
#include "../algorithms/lfu/Lfu.h"
#include "../algorithms/random_replacement/RandomReplacement.h"
#include "../algorithms/s3_fifo/S3Fifo.h"
#include "../core/disk_io/DiskIo.h"
#include <iostream>
#include <fstream>
#include <map>
#include <chrono>  
#include <iomanip>
#include <algorithm>
#include <sstream>

Simulator::Simulator() : testCapacity(10), numUsers(3), requestsPerTest(200), 
                        rng(std::random_device{}()),
                        uniformDist(1, 100),
                        poissonDist(50) {
    cacheTypes = {"lfu", "random", "s3_fifo"};
}

Simulator::~Simulator() {
    // Destructor
}
std::string PATH = "../winner_algorithm.txt";
std::string Simulator::runSimulation() {
    std::cout << "\n=== STARTING CACHE SIMULATION MODE ===\n";
    std::cout << "Testing " << cacheTypes.size() << " algorithms with " << numUsers << " users\n";
    std::cout << "Each test: " << requestsPerTest << " requests\n";
    std::cout << "Cache capacity: " << testCapacity << " files\n\n";
    
    std::vector<SimulationResult> allResults;
    
    for (const std::string& algorithm : cacheTypes) {
        std::cout << "Testing " << algorithm << " algorithm:\n";
        
        for (int userId = 1; userId <= numUsers; userId++) {
            std::cout << "  User " << userId << ":\n";
            
            auto uniformSeq = generateRandomSequence(requestsPerTest);
            auto poissonSeq = generatePoissonSequence(requestsPerTest);
            auto biasedSeq = generateBiasedSequence(requestsPerTest);
            
            std::cout << "    Testing uniform distribution...";
            auto result1 = testCacheAlgorithm(algorithm, "uniform", userId, uniformSeq);
            allResults.push_back(result1);
            std::cout << " Done\n";
            
            std::cout << "    Testing Poisson distribution...";
            auto result2 = testCacheAlgorithm(algorithm, "poisson", userId, poissonSeq);
            allResults.push_back(result2);
            std::cout << " Done\n";
            
            std::cout << "    Testing biased distribution...";
            auto result3 = testCacheAlgorithm(algorithm, "biased", userId, biasedSeq);
            allResults.push_back(result3);
            std::cout << " Done\n";
        }
        std::cout << "\n";
    }
    
    std::cout << "=== SIMULATION COMPLETE ===\n\n";
    
    printSimulationResults(allResults);
    
    std::string bestAlgorithm = selectBestAlgorithm(allResults);
    
    writeWinnerToDisk(PATH, bestAlgorithm);
    
    return bestAlgorithm;
}

std::vector<int> Simulator::generateRandomSequence(int count) {
    std::vector<int> sequence;
    sequence.reserve(count);
    
    for (int i = 0; i < count; i++) {
        sequence.push_back(uniformDist(rng));
    }
    
    return sequence;
}

std::vector<int> Simulator::generatePoissonSequence(int count) {
    std::vector<int> sequence;
    sequence.reserve(count);
    
    for (int i = 0; i < count; i++) {
        int value = poissonDist(rng);
        value = std::max(1, std::min(100, value));
        sequence.push_back(value);
    }
    
    return sequence;
}

std::vector<int> Simulator::generateBiasedSequence(int count) {
    std::vector<int> sequence;
    sequence.reserve(count);
    
    std::uniform_real_distribution<double> probDist(0.0, 1.0);
    std::uniform_int_distribution<int> biasedRange(30, 40);
    std::uniform_int_distribution<int> normalRange(1, 100);
    
    for (int i = 0; i < count; i++) {
        if (probDist(rng) < 0.43) {
            sequence.push_back(biasedRange(rng));
        } else {  // 57% chance for any file
            sequence.push_back(normalRange(rng));
        }
    }
    
    return sequence;
}

SimulationResult Simulator::testCacheAlgorithm(const std::string& algorithmName,
                                             const std::string& randomType,
                                             int userId,
                                             const std::vector<int>& sequence) {
    SimulationResult result;
    result.algorithmName = algorithmName;
    result.randomType = randomType;
    result.userId = userId;
    result.totalRequests = sequence.size();
    result.cacheHits = 0;
    result.cacheMisses = 0;
    result.totalTime = 0.0;
    
    auto cache = createTestCache(algorithmName);
    if (!cache) {
        std::cerr << "Failed to create cache for " << algorithmName << std::endl;
        return result;
    }
    
    for (int fileNumber : sequence) {
        std::string key = std::to_string(fileNumber);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        auto cachedContent = cache->get(key);
        if (cachedContent.has_value()) {
            result.cacheHits++;
        } else {
            result.cacheMisses++;
            result.missedFiles.push_back(fileNumber);
            
            std::string fileContent = readTestFile(fileNumber);
            cache->put(key, fileContent);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<double, std::milli>(end - start);
        double timeMs = elapsed.count();
        
        result.accessTimes.push_back(timeMs);
        result.totalTime += timeMs;
    }
    
    result.averageTime = result.totalTime / result.totalRequests;
    
    return result;
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

std::string Simulator::readTestFile(int fileNumber) {
    std::stringstream ss;
    ss << "Content of file " << fileNumber << ". ";
    ss << "This is simulated text content that would normally be read from disk. ";
    ss << "File " << fileNumber << " contains important legal documents. ";
    
    for (int i = 0; i < (fileNumber % 10) + 5; i++) {
        ss << "Additional content paragraph " << i << ". ";
    }
    
    return ss.str();
}

void Simulator::printSimulationResults(const std::vector<SimulationResult>& results) {
    std::cout << "=== DETAILED SIMULATION RESULTS ===\n\n";
    
    for (const auto& result : results) {
        std::cout << "Algorithm: " << result.algorithmName 
                  << " | Distribution: " << result.randomType
                  << " | User: " << result.userId << "\n";
        std::cout << "  Requests: " << result.totalRequests << "\n";
        std::cout << "  Cache Hits: " << result.cacheHits 
                  << " (" << std::fixed << std::setprecision(1) 
                  << (100.0 * result.cacheHits / result.totalRequests) << "%)\n";
        std::cout << "  Cache Misses: " << result.cacheMisses 
                  << " (" << std::fixed << std::setprecision(1) 
                  << (100.0 * result.cacheMisses / result.totalRequests) << "%)\n";
        std::cout << "  Total Time: " << std::fixed << std::setprecision(2) 
                  << result.totalTime << " ms\n";
        std::cout << "  Average Time: " << std::fixed << std::setprecision(2) 
                  << result.averageTime << " ms\n";
        std::cout << "  Unique Missed Files: " << result.missedFiles.size() << "\n\n";
    }
}

std::string Simulator::selectBestAlgorithm(const std::vector<SimulationResult>& results) {
    std::map<std::string, std::vector<double>> algorithmHitRates;
    std::map<std::string, std::vector<double>> algorithmAvgTimes;
    
    for (const auto& result : results) {
        double hitRate = (double)result.cacheHits / result.totalRequests;
        algorithmHitRates[result.algorithmName].push_back(hitRate);
        algorithmAvgTimes[result.algorithmName].push_back(result.averageTime);
    }
    
    std::cout << "=== ALGORITHM COMPARISON ===\n";
    
    std::string bestAlgorithm;
    double bestScore = -1.0;
    
    for (const auto& [algorithm, hitRates] : algorithmHitRates) {
        double avgHitRate = 0.0;
        double avgTime = 0.0;
        
        for (double rate : hitRates) {
            avgHitRate += rate;
        }
        avgHitRate /= hitRates.size();
        
        for (double time : algorithmAvgTimes[algorithm]) {
            avgTime += time;
        }
        avgTime /= algorithmAvgTimes[algorithm].size();
        
        double score = avgHitRate * 1000 - avgTime;
        
        std::cout << algorithm << ":\n";
        std::cout << "  Average Hit Rate: " << std::fixed << std::setprecision(1) 
                  << (avgHitRate * 100) << "%\n";
        std::cout << "  Average Time: " << std::fixed << std::setprecision(2) 
                  << avgTime << " ms\n";
        std::cout << "  Score: " << std::fixed << std::setprecision(2) << score << "\n\n";
        
        if (score > bestScore) {
            bestScore = score;
            bestAlgorithm = algorithm;
        }
    }
    
    std::cout << "WINNER: " << bestAlgorithm << "\n\n";
    return bestAlgorithm;
}

void runSimulationMode() {
    Simulator simulator;
    simulator.runSimulation();
}