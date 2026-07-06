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
using json = nlohmann::json;
using namespace std;
int main(int argc, char* argv[]) {
    Config models;
    cout << R"(
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
    if (argc < 3 || string(argv[1]) != "-p") {
        cerr << "Expected first argument to be '-p'" << endl;
        return 1;
    }
    string prompt = argv[2];
    if (prompt.empty()) {
        cerr << "Prompt must not be empty" << endl;
        return 1;
    }
    json request_body = {
        {"model", models.chatModel},
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
                    {"required", json::array({"command"})},
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
        ChatStatus status = chat(request_body);
        if (status == ChatStatus::Error){
            return 1;
        }
        else if (status == ChatStatus::Finished){
            break;
        }
        else{
            continue;
        }
    }
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    cerr << "Logs from your program will appear here!" << endl;
    if (request_body["messages"].contains("content") && request_body["message"]["content"].is_string()) {
        string content = request_body["messages"]["content"];
        cout << content << '\n';
    }
    return 0;
}
