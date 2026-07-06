#pragma once

#include <filesystem>
#include <string>
#include <vector>
using namespace std;
using json = nlohmann::json;
json readFile(const string& filepath, const string& tool_call_id);
json writeFile(const string& fullpath, const string& contents, const string& tool_call_id);
json exec(const string& cmd, const string& tool_call_id);
vector<float> getEmbedding(const string& text);
ChatStatus chat(json& request_body);