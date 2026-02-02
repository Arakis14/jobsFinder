#include <array>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <sqlite3.h>

using json = nlohmann::json;

static int callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}

const std::string tableQuery = "CREATE TABLE JOBS("
                            "ID             INT           PRIMARY KEY, "
                            "URL            VARCHAR(64)   UNIQUE NOT NULL, "
                            "TITLE          VARCHAR(32)   NOT NULL, "
                            "COMPANY        VARCHAR(32)   NOT NULL, "
                            "CATEGORY       VARCHAR(32)   , "
                            "DATE           VARCHAR(32)   , "
                            "LOCATION       VARCHAR(32)   , "
                            "POSITION       VARCHAR(32)   , "
                            "SKILLS         VARCHAR(64)   NOT NULL, "
                            "SALARY         VARCHAR(32));";
//TODO: dynamicpath
const std::filesystem::path jsonPath = "../sample.json";
const std::string selectAllQ = "SELECT * FROM JOBS;";
const std::string insertPrefix = "INSERT INTO JOBS VALUES";
const char* countId = "SELECT COUNT(*) FROM JOBS;";
//TODO: dynamicpath
const std::string dBPath = "example.db";
sqlite3* DB;


void openDB()
{
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
}

void closeDB()
{
    int exit = 0;
    exit = sqlite3_close(DB);
    if (exit)
    {
        std::cerr << "Error close DB " << sqlite3_errmsg(DB) << std::endl;
    }
}

void printAll()
{
    std::cout << "printAll" << std::endl;
    openDB();
    sqlite3_exec(DB, selectAllQ.c_str(), callback, NULL, NULL);
    closeDB();
}

void createDBandTable()
{
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);
    if (exit)
    {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
    }
    char* errorMsg;
    exit = sqlite3_exec(DB, tableQuery.c_str(), NULL, 0, &errorMsg);
    if (exit != SQLITE_OK)
    {
        std::cerr << "Error Creating Table: " << errorMsg  << std::endl;
        sqlite3_free(errorMsg);
    }
    else
    {
        std::cout << "Table created Successfully" << std::endl;
    }

    sqlite3_close(DB);
}

std::array<std::string, 9>  parseDataFromJsonToString(const json& data) noexcept
{
    std::array<std::string, 9> values;
    try
    {
        values.at(0) = data.at("url");
        values.at(1) = data.at("title");
        values.at(2) = data.at("company");
        values.at(3) = data.at("category");
        values.at(4) = data.at("date");
        values.at(5) = data.at("location");
        values.at(6) = data.at("position");
        auto skills = data.at("skills");
        std::string tempSkills {};
        for (auto skill : skills)
        {
            tempSkills += skill;
            tempSkills += ",";
        }
        values.at(7) = tempSkills;
        values.at(8) = data.at("salary");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return values;
}

int findNextId()
{
    openDB();
    int count = -1;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(DB, countId, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Prepare failed: %s\n" << sqlite3_errmsg(DB) << std::endl;
        return 0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);  // column 0 = COUNT(*)
    } else {
        std::cerr << "Step failed: %s\n" << sqlite3_errmsg(DB) << std::endl;
        //printf("Step failed: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    closeDB();

    return count;
}

std::string stringMagicAndIncrement(int id)
{
    //incrementing ID here
    std::string query = std::to_string(++id);
    query.insert(0,1,'(');
    query.push_back(',');
    return query;
}

void insertJobIntoTable(const std::array<std::string, 9>& values)
{
    auto idCount = findNextId();
    if (idCount == -1)
    {
        std::cerr << "Find number of IDs failed. Cannot continue..." << std::endl;
        return;
    }
    std::string query = stringMagicAndIncrement(idCount);
    //DEBUG
    //std::cout << "COUNT: " << count << std::endl;
    for (auto value : values)
    {
        query += "\'" + value + "\',";
    }
    //last ',' needs to go.
    query.pop_back();
    query += ");";
    std::string sql(insertPrefix + query);
    //DEBUG
    //std::cout << "SQL: " << sql << std::endl;

    int exit = 0;
    openDB();
    char* errorMsg;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errorMsg);
    if (exit != SQLITE_OK)
    {
        std::cerr << "Error Insert: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
    else
    {
        std::cout << "Records created Successfully!" << std::endl;
    }
    closeDB();
}

void deleteJson()
{
    std::cout << "remove(): " << std::filesystem::remove(jsonPath) << '\n';
}

int main()
{
    if(!(std::filesystem::exists(jsonPath)))
    {
        std::cerr << "No JSON!" << '\n';
        return 0;
    }
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    //std::cout << std::setw(4) << data << std::endl;
    auto dataArr = parseDataFromJsonToString(data);
    createDBandTable();
    insertJobIntoTable(dataArr);
    deleteJson();
    //DEBUG
    //printAll();
}