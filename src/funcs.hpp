#include <filesystem>
using namespace std;
using json = nlohmann::json;
json readFile(const string& filepath, const string& tool_call_id);
json writeFile(const string& fullpath, const string& contents, const string& tool_call_id);