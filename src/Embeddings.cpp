#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
#include "Chunk.hpp"
#include "Configs.hpp"
using namespace std;
using json = nlohmann::json;
vector<float> getEmbedding(const string& text) {
    Config models;
    if (text.empty()) {
        throw invalid_argument("Input text cannot be empty.");
    }
    json payload = {{"model", models.embeddingModel},
                    {"input", text}};
    cpr::Response response = cpr::Post(
                        cpr::Url{models.baseUrl + "/api/embed"},
                        cpr::Header{
                            {"Content-Type", "application/json"}
                        },
                        cpr::Body{payload.dump()}
                    );
    if (response.status_code != 200) {
        throw runtime_error("Embedding request failed: " + response.text);
    }
    json result = json::parse(response.text);
    if (!result.contains("embeddings")){
        throw runtime_error("No embedding returned.");
    }
    vector<float> embedding = result["embeddings"][0].get<vector<float>>();
    return embedding;
}