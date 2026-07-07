#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;
string readFiles(const fs::path& path) {
    ifstream file(path);
    if (!file){
        throw runtime_error("Couldn't open " + path.string());
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}