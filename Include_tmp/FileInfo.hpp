#ifndef FILEINFO_H
#define FILEINFO_H

#include <string>
#include <vector>
#include <filesystem>
#include "Chunk.hpp"

struct FileInfo {
    std::string source;
    std::filesystem::file_time_type last_modified;
};

#endif