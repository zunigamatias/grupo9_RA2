#ifndef DISK_IO_H
#define DISK_IO_H

#include <string>
#include <fstream>


void writeWinnerToDisk(const std::string& path, const std::string& algorithm);
std::string readWinnerFromDisk(const std::string& path);
std::string readTextFile(std::string path);

#endif