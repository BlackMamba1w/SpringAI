#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
using namespace std;
using json = nlohmann::json;
json writeFile(const string& fullpath, const string& contents, const string& tool_call_id){
    ofstream file(fullpath, ios::out, ios::trunc);
    if (!file) {
        cerr << "Error: Could not open file '" << filename << "' for clearing.\n";
        return {
            {"role", "tool"},
            {"tool_call_id", tool_call_id},
            {"content", "Requested file not found"}
        };
    }
    file << contents;
    return {
        {"role", "tool"},
        {"tool_call_id", tool_call_id},
        {"content", "Contents written to " + fullpath}
    };
}