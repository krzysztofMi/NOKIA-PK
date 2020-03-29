#pragma once

#include "IDatabase.hpp"
#include "Logger/PrefixedLogger.hpp"
namespace ue {

class SmsDatabase : public IDatabase{
public:
    //TODO I think we should move these error classes to another file.
    class ConnectionError : std::runtime_error{
    public:
        using std::runtime_error::runtime_error;
    };
    class QueryError : std::runtime_error{
    public:
        using std::runtime_error::runtime_error;
    };
    SmsDatabase(common::ILogger& logger);
    ~SmsDatabase() override;
    void executeQuery(std::string query,
                      int (*callback)(void *, int, char **, char **),
                      void * callArg);
private:
    void connect() override;
    common::PrefixedLogger logger;
    //Path to db. Relative path from build directory maybe this path need to be read from file which would not be push to git.
    const std::string databasePath = "../../NOKIA-PK/UE/Application/Database/Schema/smsdatabase.db";
    sqlite3 *db;
};

}
