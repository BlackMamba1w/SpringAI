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
using namespace std;
using json = nlohmann::json;
string getLanguage(const fs::path& path) {
    string ext = path.extension().string();
    if (ext == ".cpp" || ext == ".cc" || ext == ".cxx")
        return "cpp";
    if (ext == ".hpp" || ext == ".h")
        return "cpp";
    if (ext == ".py")
        return "python";
    if (ext == ".java")
        return "java";
    if (ext == ".js")
        return "javascript";
    if (ext == ".ts")
        return "typescript";
    if (ext == ".cs")
        return "csharp";
    if (ext == ".go")
        return "go";
    if (ext == ".rs")
        return "rust";
    if (ext == ".php")
        return "php";
    if (ext == ".html")
        return "html";
    if (ext == ".css")
        return "css";
    if (ext == ".md")
        return "markdown";
    return "text";
}
vector<Chunk> getChunks(const string& text1, const fs::path& path, size_t chunk_size = 800, size_t overlap = 150){
    size_t pos = 0;
    int idx = 0;
    vector<Chunk> chunks;
    while (pos < text1.size()) {
        size_t end = std::min(pos + chunk_size, text1.size());
        // try not to cut mid-word: walk back to nearest space/newline
        if (end < text1.size()) {
            size_t back = end;
            while (back > pos && !isspace((unsigned char)text1[back])) back--;
            if (back > pos) end = back;
        }
        Chunk chunk;
        string source = path.string();
        chunk.startPos = pos;
        chunk.endPos = end;
        chunk.id = source + "_Chunk_" + to_string(idx++);
        chunk.language = getLanguage(path);
        chunk.source = source;
        chunk.text = text1.substr(pos, end);
        chunk.embed = getEmbedding(text1.substr(pos, end));
        chunks.push_back(chunk);
    }
    return chunks;
}
void saveChunks(const vector<Chunk>& chunks){
    json root = json::array();
    for (const auto& chunk : chunks) {
        root.push_back({
            {"id", chunk.id},
            {"source", chunk.source},
            {"language", chunk.language},
            {"text", chunk.text},
            {"embed", chunk.embed}
        });
    }
    ofstream out("data/chunks.json");
    out << root.dump(4);
}
vector<Chunk> loadChunks(){
    ifstream in("data/chunks.json");
    json root;
    in >> root;
    vector<Chunk> chunks;
    for (const auto& item : root) {
        Chunk chunk;
        chunk.id = item["id"];
        chunk.source = item["source"];
        chunk.language = item["language"];
        chunk.text = item["text"];
        chunk.embed = item["embed"];
        chunks.push_back(chunk);
    }
    return chunks;
}