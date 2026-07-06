#include <fstream>
#include <sstream>
using namespace std;
string readFiles(const fs::path& path) {
    ifstream file(path);
    if (!file)
        return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}