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
vector<Chunk> getChunks(const string& text1, const string& source, size_t chunk_size = 800, size_t overlap = 150);
string getLanguage(const filesystem::path& path);
void saveChunks(const vector<Chunk>& chunks);
vector<Chunk> loadChunks();
float similarity(const vector<float>& A, const vector<float>& B){