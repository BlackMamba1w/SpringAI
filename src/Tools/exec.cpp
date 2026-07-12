#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif
using namespace std;
using json = nlohmann::json;
json exec(const string& cmd, const string& tool_call_id){
    array<char, 128> buffer;
    string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe){
        return {
            {"role", "tool"},
            {"tool_call_id", tool_call_id},
            {"content", "Failed to execute command. Please try again with a different command."}
        };
    }
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr){
        result += buffer.data();
    }
    pclose(pipe);
    return {
            {"role", "tool"},
            {"tool_call_id", tool_call_id},
            {"content", "Command executed successfully. Output is as follows: " + result}
    };
}