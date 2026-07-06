#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    string chatModel;
    string embeddingModel;
    string baseUrl;

    Config(
        const string& chat = "mistral:latest",
        const string& embed = "nomic-embed-text",
        const string& url = "http://localhost:11434/v1"): 
        chatModel(chat),
        embeddingModel(embed),
        baseUrl(url) {}
};

#endif