#include <string>
#include <fstream>


void writeWinnerToDisk(const std::string& path, const std::string& algorithm) {
    std::ofstream file(path, std::ios::trunc);
    if (file.is_open()) {
        file << algorithm << std::endl;
        file.close();
    }
}

std::string readWinnerFromDisk(const std::string& path) {
    std::ifstream file(path);
    std::string name;
    if (file.is_open()) {
        std::getline(file, name);
        file.close();
    }
    return name;
}