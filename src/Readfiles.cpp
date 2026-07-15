#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;
string readFiles(const fs::path& path) {
    cout << "Reading: " << path << endl;
    ifstream file(path);
    if (!file){
        throw runtime_error("Couldn't open " + path.string());
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}