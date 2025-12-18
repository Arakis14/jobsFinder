#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <sqlite3.h>

using json = nlohmann::json;

const std::string table =   "CREATE TABLE JOBS("
                            "ID INT PRIMARY KEY     NOT NULL, "
                            "LINK           TEXT    NOT NULL, "
                            "ROLE           TEXT    NOT NULL, "
                            "BRANCH         TEXT    NOT NULL, "
                            "DATE           DATE    NOT NULL, "
                            "LOCATION       TEXT    NOT NULL, "
                            "LEVEL          TEXT    NOT NULL, "
                            "SALLARY        REAL    NOT NULL, "
                            "SKILLS         TEXT    NOT NULL);";
//TODO: dynamicpath
const std::filesystem::path jsonPath = "../sample.json";

void createDBandTable()
{
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);
    if (exit)
    {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
    }
    else
    {
        std::cout << "Opened Database Successfully!" << std::endl;
    }
    char* errorMsg;
    exit = sqlite3_exec(DB, table.c_str(), NULL, 0, &errorMsg);
    if (exit != SQLITE_OK)
    {
        std::cerr << "Error Creating Table" << std::endl;
        sqlite3_free(errorMsg);
    }
    else
    {
        std::cout << "Table created Successfully" << std::endl;
    }

    sqlite3_close(DB);
}

std::string parseDataFromJson(const json& data)
{

}

void insertIntoTable(const std::string& str)
{

}

int main()
{
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    std::cout << std::setw(4) << data << std::endl;
    createDBandTable();
}