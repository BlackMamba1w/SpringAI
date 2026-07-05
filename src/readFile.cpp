#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
using namespace std;
using json = nlohmann::json;
void readFile(const string& filepath, const string& tool_call_id){
    if (filesystem::exists(filepath)){
        ifstream inFile(filepath);
        string data;
        string line;
        json toolResponse;
        while (getline(inFile, line)){
            data += line + "\n"
        }
        toolResponse = {
            {"role", "tool"},
            {"tool_call_id", tool_call_id},
            {"content", data}
        }
        return toolResponse;
    }
    else{
        return "Requested file not found"
    }
}