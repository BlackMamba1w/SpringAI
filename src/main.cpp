#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "funcs.hpp"
using json = nlohmann::json;
using namespace std;
int main(int argc, char* argv[]) {
    /* std::cout << R"(
    ╭──────────────────────────────────────────────────────────────╮
    │                                                              │
    │   ███████╗██████╗ ██████╗ ██╗███╗   ██╗ ██████╗  █████╗ ██╗  │
    │   ██╔════╝██╔══██╗██╔══██╗██║████╗  ██║██╔════╝ ██╔══██╗██║  │
    │   ███████╗██████╔╝██████╔╝██║██╔██╗ ██║██║  ███╗███████║██║  │
    │   ╚════██║██╔═══╝ ██╔══██╗██║██║╚██╗██║██║   ██║██╔══██║██║  │
    │   ███████║██║     ██║  ██║██║██║ ╚████║╚██████╔╝██║  ██║██║  │
    │   ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  │
    │                                                              │
    │          AI-powered coding from your terminal.               │
    │                                                              │
    ╰──────────────────────────────────────────────────────────────╯
    )"; 
    */
    if (argc < 3 || string(argv[1]) != "-p") {
        cerr << "Expected first argument to be '-p'" << endl;
        return 1;
    }
    string prompt = argv[2];
    if (prompt.empty()) {
        cerr << "Prompt must not be empty" << endl;
        return 1;
    }
    const char* api_key_env = getenv("OPENROUTER_API_KEY");
    const char* base_url_env = getenv("OPENROUTER_BASE_URL");
    string api_key = api_key_env ? api_key_env : "";
    string base_url = base_url_env ? base_url_env : "https://openrouter.ai/api/v1";
    if (api_key.empty()) {
        cerr << "OPENROUTER_API_KEY is not set" << endl;
        return 1;
    }
    json request_body = {
        {"model", "anthropic/claude-haiku-4.5"},
        {"messages", json::array({
            {
                {"role", "user"}, 
                {"content", prompt}
            }
        })},
        {"tools", json::array({
            {
                {"type", "function"},
                {"function", {
                    {"name", "Read"},
                    {"description", "Read and return the contents of a file"},
                    {"parameters", {
                        {"type", "object"},
                        {"properties", {
                            {"file_path", {
                                {"type", "string"},
                                {"description", "The path to the file to read"}
                            }}
                        }},
                        {"required", json::array({"file_path"})}
                    }}
                }}
            }
        })}
    };
    while (message.contains("tool_calls") && !message["tool_calls"].is_null()){
        json toolcall;
        string tool;
        string args;
        json args_data;
        cpr::Response response = cpr::Post(
            cpr::Url{base_url + "/chat/completions"},
            cpr::Header{
                {"Authorization", "Bearer " + api_key},
                {"Content-Type", "application/json"}
            },
            cpr::Body{request_body.dump()}
        );
        if (response.status_code != 200) {
            cerr << "HTTP error: " << response.status_code << endl;
            return 1;
        }
        json response = json::parse(response.text);
        if (!response.contains("choices") || response["choices"].empty()) {
            cerr << "No choices in response" << endl;
            return 1;
        }
        request_body["messages"].push_back(response)
        if (response.contains("tool_calls")){
            toolcall = response["choices"][0]["message"]["tool_calls"][0];
            tool = toolcall["function"]["name"].get<string>();
            args = response["choices"][0]["message"]["tool_calls"][0]["function"]["arguments"];
            args_data = json::parse(args);
            if (tool == "Read"){
                filepath = args_data["file_path"];
                json toolResponse = readFile(filepath, toolcall["id"].get<string>());
                request_body["messages"].push_back(toolResponse)
            }
        }
    }
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    cerr << "Logs from your program will appear here!" << endl;
    if (request_body["message"].contains("content") && request_body["message"]["content"].is_string()) {
        string content = request_body["message"]["content"];
        cout << content << '\n';
    }
    return 0;
}