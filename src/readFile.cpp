#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
using namespace std;
using json = nlohmann::json;
json readFile(const string& filepath, const string& tool_call_id) {
    ifstream inFile(filepath);
    if (!inFile.is_open()) {
        return {
            {"role", "tool"},
            {"tool_call_id", tool_call_id},
            {"content", "Requested file not found"}
        };
    }
    string data;
    string line;
    while (getline(inFile, line)) {
        data += line + "\n";
    }
    return {
        {"role", "tool"},
        {"tool_call_id", tool_call_id},
        {"content", data}
    };
}