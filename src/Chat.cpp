#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
#include <vector>
#include "Configs.hpp"
using json = nlohmann::json;
using namespace std;
enum class ChatStatus {
    Continue,
    Finished,
    Error
};
ChatStatus chat(json& request_body){
    json toolcall;
    string tool;
    string args;
    json args_data;
    string filepath;
    string contents;
    Config models;
    cpr::Response http = cpr::Post(
            cpr::Url{models.baseUrl + "/chat/completions"},
            cpr::Header{
                {"Authorization", "Bearer " + models.apiKey},
                {"Content-Type", "application/json"}
            },
            cpr::Body{request_body.dump()}
        );
    if (http.status_code != 200) {
        cerr << "HTTP error: " << http.status_code << endl;
        return ChatStatus::Error;
    }
    json response = json::parse(http.text);
    if (!response.contains("choices") || response["choices"].empty()) {
        cerr << "No choices in response" << endl;
        return ChatStatus::Error;
    }
    json message = response["choices"][0]["message"];
    if (!message.contains("tool_calls") || message["tool_calls"].is_null()) {
        cout << '\n\n\n' << message["content"].get<string>() << '\n';
        return ChatStatus::Finished;
    }
    request_body["messages"].push_back(message);
    toolcall = message["tool_calls"][0];
    tool = toolcall["function"]["name"].get<string>();
    args_data = json::parse(message["tool_calls"][0]["function"]["arguments"]);
    if (tool == "Read"){
        filepath = args_data["file_path"];
        json toolMessage = readFile(filepath, toolcall["id"].get<string>());
        request_body["messages"].push_back(toolMessage);
        return ChatStatus::Continue;
    }
    else if(tool == "Write"){
        filepath = args_data["file_path"];
        contents = args_data["content"];
        json toolMessage = writeFile(filepath, contents, toolcall["id"].get<string>());
        request_body["messages"].push_back(toolMessage);
        return ChatStatus::Continue;
    }
    else if(tool == "Bash"){
        json toolMessage = exec(args_data["command"], toolcall["id"].get<string>());
        request_body["messages"].push_back(toolMessage);
        return ChatStatus::Continue;
    }
}