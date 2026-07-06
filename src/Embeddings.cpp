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
    json payload = {{"model", models.embeddingModel},
                    {"prompt", text}}
    auto response = cpr::Post(
                        cpr::Url{base_url + "/api/embeddings"},
                        cpr::Header{
                            {"Content-Type", "application/json"}
                        },
                        cpr::Body{payload.dump()}
                    );
}