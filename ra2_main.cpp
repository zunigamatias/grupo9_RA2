#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

// local cache algorithms
#include "algorithms/Cache.h"
#include "algorithms/lfu/Lfu.h"
#include "algorithms/random_replacement/RandomReplacement.h"
#include "algorithms/s3_fifo/S3Fifo.h"
#include "simulation/Simulator.h"

// disk read and write functions
#include "core/disk_io/DiskIo.h"

std::unique_ptr<Cache> createCache(const std::string& cacheType, int capacity) {  
    if (cacheType == "lfu") {
        return std::make_unique<Lfu>(capacity);
    } else if (cacheType == "random") {
        return std::make_unique<RandomReplacement>(capacity);
    } else if (cacheType == "s3_fifo") {
        return std::make_unique<S3Fifo>(capacity);
    }
    
    return nullptr;
}

int main() {
    const std::string configFile = "../winner_algorithm.txt";

    std::cout << "=== Text Cache System ===\n";
    
    std::string algorithm = readWinnerFromDisk(configFile);
    std::cout << "algorithm: " << algorithm << std::endl;
    if (algorithm.empty()) {
        std::cout << "No winner algorithm found. Please run simulation mode first (-1).\n";
    }
    
    std::unique_ptr<Cache> cache = createCache(algorithm, 10);
    if (!cache && !algorithm.empty()) {
        std::cerr << "Error: could not initialize cache for algorithm: " << algorithm << "\n";
        return 1;
    }
    
    while (true) {
        std::cout << "\nEnter text number (1-100), 0 to exit, -1 for simulation mode:\n> ";
        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Input failed, you must enter a number\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        if (choice != -1 && algorithm.empty()) {
            std::cout << "\nNo winner algorithm found. Please run simulation mode first (-1).";
            continue;
        }

        if (choice == 0) {
            std::cout << "\nExiting program...";
            break;
        }

        if (choice == -1) {
            std::cout << "\nEntering simulation mode...\n";
            runSimulationMode();

            algorithm = readWinnerFromDisk(configFile);
            std::cout << "Simulation complete. Winner algorithm: " << algorithm << "\n";
            cache = createCache(algorithm, 10);
            continue;
        }

        if (choice < 1 || choice > 100) {
            std::cout << "Invalid text number. Please enter between 1 and 100.\n";
            continue;
        }

        std::string fileName = "text_" + std::to_string(choice) + ".txt";

        auto start = std::chrono::high_resolution_clock::now();
        bool hit = false;
        std::string strChoice = std::to_string(choice);
        auto item = cache->get(strChoice);

        if (item.has_value()) {
            hit = true;
            std::cout << "[Cache HIT]\n";
        } else {
            hit = false;
            std::cout << "[Cache MISS]\n";
            std::string fileContent = readTextFile(fileName);
            cache->put(strChoice, fileContent);
            
            item = cache->get(strChoice);
            if (!item.has_value()) {
                std::cout << "unable to read file after put, closing program\n";
                exit(-1);
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "Text " << choice << " loaded in " << elapsed.count() << " ms.\n";

        std::cout << "---- Full text ----\n";
        std::cout << item.value() << "...\n";
    }

    return 0;
}
