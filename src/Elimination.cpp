#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
#include "Configs.hpp"
#include "Chunk.hpp"
namespace fs = std::filesystem;
using namespace std;
using json = nlohmann::json;
bool passFile(const fs::path& path){
    static const set<string> exts = {
        ".cpp", ".hpp", ".h", ".c",
        ".py", ".java", ".js", ".ts",
        ".rs", ".go", ".md", ".txt"
        ".json", ".yaml", ".yml",
        ".cmake"
    };
    bool b1 = exts.contains(path.extension().string());
    bool b2 = fs::is_regular_file(path);
    bool b3;
    static const set<string> ignored = {
        ".git",
        ".vscode",
        "build",
        "bin",
        "out",
        "node_modules",
        ".idea",
        ".cache",
        "vcpkg_installed",
        ".github"
    };
    for (const auto& part : path) {
        if (ignored.contains(part.string()))
            return false;
    }
    return b1 && b2;
}