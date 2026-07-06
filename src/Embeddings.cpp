#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
using namespace std;
using json = nlohmann::json;
vector<float> getEmbedding(const string& text) {
    Config models;
    if (text.empty()) {
        throw invalid_argument("Input text cannot be empty.");
    }
    if (response.status_code != 200) {
        throw runtime_error("Embedding request failed: " + response.text);
    }
    if (!response.contains["embedding"]){
        throw runtime_error("No embedding returned.");
    }
    json payload = {{"model", models.embeddingModel},
                    {"prompt", text}};
    cpr::Response response = cpr::Post(
                        cpr::Url{models.baseUrl + "/api/embeddings"},
                        cpr::Header{
                            {"Content-Type", "application/json"}
                        },
                        cpr::Body{payload.dump()}
                    );
    json result = json::parse(response.text);
    vector<float> embedding = result["embedding"].get<vector<float>>();
    return embedding;
}