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
void Simulator::generateGnuplotGraphics(const std::vector<SimulationResult>& results) {
    std::cout << "Generating gnuplot graphics...\n";
    
    createHitRateChart(results);
    createTimeChart(results);
    createDistributionChart(results);
    createMissAnalysisChart(results);
    
    std::cout << "Graphics generated! Check for PNG files in current directory:\n";
    std::cout << "  - hit_rates.png\n";
    std::cout << "  - response_times.png\n";
    std::cout << "  - distribution_analysis.png\n";
    std::cout << "  - miss_analysis.png\n\n";
}

void Simulator::createHitRateChart(const std::vector<SimulationResult>& results) {
    // Calculate average hit rates by algorithm
    std::map<std::string, std::vector<double>> algorithmHitRates;
    
    for (const auto& result : results) {
        double hitRate = (double)result.cacheHits / result.totalRequests * 100;
        algorithmHitRates[result.algorithmName].push_back(hitRate);
    }
    
    // Prepare data
    std::vector<std::pair<std::string, double>> chartData;
    for (const auto& [algorithm, rates] : algorithmHitRates) {
        double sum = 0;
        for (double rate : rates) sum += rate;
        double avgRate = sum / rates.size();
        chartData.push_back({algorithm, avgRate});
    }
    
    // Write data to file
    writeDataToFile("../results/hit_rates_data.txt", chartData);
    
    // Create gnuplot script
    std::ofstream script("hit_rates_plot.gnu");
    script << "set terminal png size 800,600\n";
    script << "set output '../results/hit_rates.png'\n";
    script << "set title 'Cache Hit Rate Comparison by Algorithm'\n";
    script << "set xlabel 'Algorithm'\n";
    script << "set ylabel 'Hit Rate (%)'\n";
    script << "set style data histograms\n";
    script << "set style histogram cluster gap 1\n";
    script << "set style fill solid border -1\n";
    script << "set boxwidth 0.8\n";
    script << "set grid y\n";
    script << "set yrange [0:100]\n";
    script << "plot 'hit_rates_data.txt' using 2:xtic(1) title 'Hit Rate' linecolor rgb '#2E8B57'\n";
    script.close();
    
    // Execute gnuplot
    system("gnuplot hit_rates_plot.gnu");
}

void Simulator::createTimeChart(const std::vector<SimulationResult>& results) {
    // Calculate average response times by algorithm
    std::map<std::string, std::vector<double>> algorithmTimes;
    
    for (const auto& result : results) {
        algorithmTimes[result.algorithmName].push_back(result.averageTime);
    }
    
    // Prepare data
    std::vector<std::pair<std::string, double>> chartData;
    for (const auto& [algorithm, times] : algorithmTimes) {
        double sum = 0;
        for (double time : times) sum += time;
        double avgTime = sum / times.size();
        chartData.push_back({algorithm, avgTime});
    }
    
    // Write data to file
    writeDataToFile("times_data.txt", chartData);
    
    // Create gnuplot script
    std::ofstream script("times_plot.gnu");
    script << "set terminal png size 800,600\n";
    script << "set output '../results/response_times.png'\n";
    script << "set title 'Average Response Time by Algorithm'\n";
    script << "set xlabel 'Algorithm'\n";
    script << "set ylabel 'Response Time (ms)'\n";
    script << "set style data histograms\n";
    script << "set style histogram cluster gap 1\n";
    script << "set style fill solid border -1\n";
    script << "set boxwidth 0.8\n";
    script << "set grid y\n";
    script << "plot 'times_data.txt' using 2:xtic(1) title 'Avg Time' linecolor rgb '#DC143C'\n";
    script.close();
    
    // Execute gnuplot
    system("gnuplot times_plot.gnu");
}

void Simulator::createDistributionChart(const std::vector<SimulationResult>& results) {
    // Group data by algorithm and distribution
    std::map<std::string, std::map<std::string, std::vector<double>>> groupedData;
    
    for (const auto& result : results) {
        double hitRate = (double)result.cacheHits / result.totalRequests * 100;
        groupedData[result.algorithmName][result.randomType].push_back(hitRate);
    }
    
    // Write data file for each algorithm
    std::ofstream dataFile("distribution_data.txt");
    dataFile << "Distribution LFU Random S3-FIFO\n";
    
    for (const std::string& dist : {"uniform", "poisson", "biased"}) {
        dataFile << dist;
        for (const std::string& alg : {"lfu", "random", "s3_fifo"}) {
            if (groupedData[alg].find(dist) != groupedData[alg].end()) {
                double sum = 0;
                for (double rate : groupedData[alg][dist]) sum += rate;
                double avg = sum / groupedData[alg][dist].size();
                dataFile << " " << avg;
            } else {
                dataFile << " 0";
            }
        }
        dataFile << "\n";
    }
    dataFile.close();
    
    // Create gnuplot script
    std::ofstream script("distribution_plot.gnu");
    script << "set terminal png size 1000,600\n";
    script << "set output '../results/distribution_analysis.png'\n";
    script << "set title 'Hit Rate by Distribution Type and Algorithm'\n";
    script << "set xlabel 'Distribution Type'\n";
    script << "set ylabel 'Hit Rate (%)'\n";
    script << "set style data histograms\n";
    script << "set style histogram cluster gap 1\n";
    script << "set style fill solid border -1\n";
    script << "set boxwidth 0.8\n";
    script << "set grid y\n";
    script << "set key outside right\n";
    script << "plot 'distribution_data.txt' using 2:xtic(1) title 'LFU' linecolor rgb '#2E8B57', \\\n";
    script << "     '' using 3 title 'Random' linecolor rgb '#DC143C', \\\n";
    script << "     '' using 4 title 'S3-FIFO' linecolor rgb '#4169E1'\n";
    script.close();
    
    // Execute gnuplot
    system("gnuplot distribution_plot.gnu");
}

void Simulator::createMissAnalysisChart(const std::vector<SimulationResult>& results) {
    // Count total misses per algorithm
    std::map<std::string, int> totalMisses;
    
    for (const auto& result : results) {
        totalMisses[result.algorithmName] += result.cacheMisses;
    }
    
    // Prepare data
    std::vector<std::pair<std::string, double>> chartData;
    for (const auto& [algorithm, misses] : totalMisses) {
        chartData.push_back({algorithm, (double)misses});
    }
    
    // Write data to file
    writeDataToFile("misses_data.txt", chartData);
    
    // Create gnuplot script
    std::ofstream script("misses_plot.gnu");
    script << "set terminal png size 800,600\n";
    script << "set output '../results/miss_analysis.png'\n";
    script << "set title 'Total Cache Misses by Algorithm'\n";
    script << "set xlabel 'Algorithm'\n";
    script << "set ylabel 'Total Misses'\n";
    script << "set style data histograms\n";
    script << "set style histogram cluster gap 1\n";
    script << "set style fill solid border -1\n";
    script << "set boxwidth 0.8\n";
    script << "set grid y\n";
    script << "plot 'misses_data.txt' using 2:xtic(1) title 'Total Misses' linecolor rgb '#FF8C00'\n";
    script.close();
    
    system("gnuplot misses_plot.gnu");
}

void Simulator::writeDataToFile(const std::string& filename, 
                               const std::vector<std::pair<std::string, double>>& data) {
    std::ofstream file(filename);
    for (const auto& [label, value] : data) {
        file << label << " " << value << "\n";
    }
    file.close();
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
        
    // Generate external graphics with gnuplot
    generateGnuplotGraphics(allResults);
    
    // Print detailed results
    printSimulationResults(allResults);
    
    // Select best algorithm
    std::string bestAlgorithm = selectBestAlgorithm(allResults);
    
    // Save winner
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
        auto elapsed = std::chrono::duration<double, std::micro>(end - start);  // Use microseconds
        double timeMicroseconds = elapsed.count();
        double timeMs = timeMicroseconds / 1000.0;  // Convert to milliseconds
        
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
        std::cout << "  Total Time: " << std::fixed << std::setprecision(4)
                  << result.totalTime << " ms\n";
        std::cout << "  Average Time: " << std::fixed << std::setprecision(4)
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
        std::cout << "  Average Time: " << std::fixed << std::setprecision(4)
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