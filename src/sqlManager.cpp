#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

//TODO: dynamicpath
const std::filesystem::path jsonPath = "../sample.json";

int main()
{
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    std::cout << std::setw(4) << data << std::endl;
}