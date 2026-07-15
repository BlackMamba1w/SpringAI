#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include "funcs.hpp"
#include <vector>
#include "Configs.hpp"
#include "Chunk.hpp"
#include "FileInfo.hpp"
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
static constexpr size_t DEFAULT_CHUNK_SIZE = 800;
static constexpr size_t DEFAULT_OVERLAP = 150;
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
vector<Chunk> getChunks(const string& text1, const fs::path& path, size_t chunk_size, size_t overlap) {
    size_t pos = 0;
    int idx = 0;
    vector<Chunk> chunks;
    int i = 0;
    while (pos < text1.size()) {
        size_t total = (text1.size() + chunk_size - overlap - 1) / (chunk_size - overlap);
        size_t current = idx + 1;
        cout << "Embedding "
            << current
            << "/"
            << total
            << endl;
        size_t end = min(pos + chunk_size, text1.size());
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
        chunk.text = text1.substr(pos, end - pos);
        if (path.filename() == "README.md" && idx == 2) {
            cout << "Chunk contents:\n";
            cout << chunk.text << endl;

            cout << "\nBytes:\n";
            for (unsigned char c : chunk.text) {
                printf("%02X ", c);
            }
            cout << endl;
        }
        chunk.embed = getEmbedding(chunk.text);
        cout << "F";
        chunks.push_back(chunk);
        if (end == text1.size()) {
            break;
        }
        pos = (end > overlap) ? end - overlap : end;
    }
    return chunks;
}
FileInfo saveFileInfo(const fs::directory_entry& entr) {
    FileInfo info;
    info.last_modified = fs::last_write_time(entr.path());
    info.source = entr.path().string();
    return info;
}
unordered_map<string, fs::file_time_type> loadFileTimes() {
    unordered_map<string, fs::file_time_type> fileTimes;
    ifstream in("data/files.json");
    if (!in) return fileTimes;
    json root;
    in >> root;
    for (const auto& item : root) {
        string source = item["source"];
        int64_t ticks = item["last_modified"];
        fileTimes[source] = fs::file_time_type(fs::file_time_type::duration(ticks));
    }
    return fileTimes;
}
void saveChunks() {
    unordered_map<string, fs::file_time_type> oldTimes = loadFileTimes();
    unordered_map<string, vector<Chunk>> chunkMap;
    vector<Chunk> oldChunks = loadChunks(); 
    for (const auto& chunk : oldChunks){
        chunkMap[chunk.source].push_back(chunk);
    }
    vector<FileInfo> infos;
    vector<Chunk> chunks;
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (passFile(entry)) {
            string source = entry.path().string();
            fs::file_time_type currentTime = fs::last_write_time(entry.path());
            auto it = oldTimes.find(source);
            bool changed = (it == oldTimes.end()) || (it->second != currentTime); 
            if (changed) {
                vector<Chunk> tempChunks = getChunks(readFiles(entry.path()), entry.path());
                chunks.insert(
                    chunks.end(),
                    tempChunks.begin(),
                    tempChunks.end()
                );
            }
            else {
                auto it = chunkMap.find(source);
                if (it != chunkMap.end()) {
                    chunks.insert(
                        chunks.end(),
                        it -> second.begin(),
                        it -> second.end()
                    );
                }
            }
            infos.push_back(saveFileInfo(entry));
        }
    }
    json root = json::array();
    for (const auto& chunk : chunks) {
        root.push_back({
            {"id", chunk.id},
            {"source", chunk.source},
            {"language", chunk.language},
            {"text", chunk.text},
            {"embed", chunk.embed},
            {"startPos", chunk.startPos},
            {"endPos", chunk.endPos}
        });
    }
    fs::create_directories("data");
    ofstream out("data/chunks.json");
    out << root.dump(4);
    json root2 = json::array();
    for (const auto& file : infos) {
        root2.push_back({
            {"last_modified", file.last_modified.time_since_epoch().count()},
            {"source", file.source}
        });
    }
    ofstream out2("data/files.json");
    out2 << root2.dump(4);
}
vector<Chunk> loadChunks() {
    ifstream in("data/chunks.json");
    if (!in) {
        return {};
    }
    json root;
    in >> root;
    vector<Chunk> chunks;
    for (const auto& item : root) {
        Chunk chunk;
        chunk.id = item["id"];
        chunk.source = item["source"];
        chunk.language = item["language"];
        chunk.text = item["text"];
        chunk.embed = item["embed"].get<vector<float>>();
        chunk.startPos = item["startPos"];
        chunk.endPos = item["endPos"];
        chunks.push_back(chunk);
    }
    return chunks;
}