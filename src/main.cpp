#include <cstdlib>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <filesystem>
using json = nlohmann::json;
using namespace std;
int main(int argc, char* argv[]) {
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

    json result = json::parse(response.text);

    if (!result.contains("choices") || result["choices"].empty()) {
        cerr << "No choices in response" << endl;
        return 1;
    }
    json toolcall = result["choices"][0]["message"]["tool_calls"][0];
    string func_name = toolcall["function"]["name"].get<string>();
    string args = result["choices"][0]["message"]["tool_calls"][0]["function"]["arguments"];
    json args_data = json::parse(args);
    string filepath = args_data["file_path"];
    // You can use print statements as follows for debugging, they'll be visible when running tests.
    cerr << "Logs from your program will appear here!" << endl;
    if (func_name == "Read"){
        if (filesystem::exists(filepath)){
            ifstream inFile(filepath);
            string line;
            while (getline(inFile, line)){
                cout << line << endl;
            }
            
        }
    }
    // TODO: Uncomment the line below to pass the first stage
    cout << result["choices"][0]["message"]["content"].get<string>();
    return 0;
}
