#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    string chatModel;
    string embeddingModel;
    string baseUrl;
    string apiKey;
    Config(
        const string& api_key = "ollama",
        const string& chat = "mistral:latest",
        const string& embed = "nomic-embed-text",
        const string& url = "http://localhost:11434/v1"): 
        apiKey(api_key)
        chatModel(chat),
        embeddingModel(embed),
        baseUrl(url) {}
};

#endif