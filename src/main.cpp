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
    /* cout << R"(
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
            },
            {
                {"type", "function"},
                {"function", {
                    {"name", "Write"},
                    {"description", "Write content to a file"},
                    {"parameters", {
                        {"type", "object"},
                        {"required", {"file_path", "content"}},
                        {"properties", {
                            {"file_path", {
                                {"type", "string"},
                                {"description", "The path of the file to write to"}
                            }},
                            {"content", {
                                {"type", "string"},
                                {"description", "The content to write to the file"}
                            }}
                        }}
                    }}
                }}
            },
            {
                {"type", "function"},
                {"function", {
                    {"name", "Bash"},
                    {"description", "Execute a shell command"},
                    {"parameters", {
                    {"type", "object"},
                    {"required", "command"},
                    {"properties", {
                        {"command", {
                        {"type", "string"},
                        {"description", "The command to execute"}
                        }}
                    }}
                    }}
                }}
            }
        })}
    };
    while (true){
        json toolcall;
        string tool;
        string args;
        json args_data;
        string filepath;
        string contents;
        cpr::Response http = cpr::Post(
            cpr::Url{base_url + "/chat/completions"},
            cpr::Header{
                {"Authorization", "Bearer " + api_key},
                {"Content-Type", "application/json"}
            },
            cpr::Body{request_body.dump()}
        );
        if (http.status_code != 200) {
            cerr << "HTTP error: " << http.status_code << endl;
            return 1;
        }
        json response = json::parse(http.text);
        if (!response.contains("choices") || response["choices"].empty()) {
            cerr << "No choices in response" << endl;
            return 1;
        }
        json message = response["choices"][0]["message"];
        if (!message.contains("tool_calls") || message["tool_calls"].is_null()) {
            cout << message["content"].get<string>() << '\n';
            break;
        }
        request_body["messages"].push_back(message);
        toolcall = message["tool_calls"][0];
        tool = toolcall["function"]["name"].get<string>();
        args = message["tool_calls"][0]["function"]["arguments"];
        args_data = json::parse(args);
        if (tool == "Read"){
            filepath = args_data["file_path"];
            json toolMessage = readFile(filepath, toolcall["id"].get<string>());
            request_body["messages"].push_back(toolMessage);
        }
        else if(tool == "Write"){
            filepath = args_data["file_path"];
            contents = args_data["content"];
            json toolMessage = writeFile(filepath, contents, toolcall["id"].get<string>());
            request_body["messages"].push_back(toolMessage);
        }
        else if(tool == "Bash"){
            json toolMessage = exec(args_data["command"]);
            request_body["messages"].push_back(toolMessage);
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
