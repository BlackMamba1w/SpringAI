#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
#include "Configs.hpp"
#include "ChatStatus.hpp"
#include "Chunk.hpp"
namespace fs = std::filesystem;
void saveFileChunks(){
    vector<Chunk> chunks;
    for (const auto& entry: fs::recursive_directory_iterator(".")){
        if (passFile(entry)){
            vector<Chunk> tempChunks = getChunks(readFiles(entry.path()), entry.path());
            for (const auto& chunk: tempChunks){
                chunks.push_back(chunk);
            }
        }
    }
}
string retrieveContext(const int& x, const vector<float>& embedding){
    vector<Chunk> chunks = loadChunks();
    int k = min(x, (int)chunks.size());
    partial_sort(
        chunks.begin(),
        chunks.begin() + k,
        chunks.end(),
        [&](const Chunk& a, const Chunk& b) {
            return similarity(embedding, a.embed) >
                similarity(embedding, b.embed);
        }
    );
    vector<Chunk> retrieved;
    float best = similarity(chunks[0].embed, embedding);
    if (best < 0.80f){
        return "";
    }
    for (int i = 0; i < k; i++){
        if(similarity(chunks[i].embed, embedding) > 0.70f || similarity(chunks[i].embed, embedding) >= best - 0.05f){
            retrieved.push_back(chunks[i]);
        }
    }
    string context;
    for (const auto& chunk : retrieved){
        context += "File: " + chunk.source + '\n';
        context += chunk.text;
        context += '\n' + '\n' + '\n';
    }
    return context;
}