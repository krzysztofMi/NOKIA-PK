#include "SmsDatabase.hpp"
namespace ue {

SmsDatabase::SmsDatabase(common::ILogger& logger) : logger(logger, "[SMS-DATABASE]")
{
    this->connect();
}

SmsDatabase::~SmsDatabase()
{
    sqlite3_close(db);
    logger.logInfo("Connection to SMS database closed.");
}

void SmsDatabase::connect()
{
    int err = sqlite3_open(databasePath.c_str(), &db);
    if(err){
        logger.logError("Error when opening database.");
        throw ConnectionError("Connection to SMS database " + databasePath + " failed.");
    }
    logger.logInfo("Succesfully connected to SMS database.");
}

void SmsDatabase::executeQuery(std::string sql,
                                int (*callback)(void *, int, char **, char **),
                                void * callArg)
{
    logger.logInfo(sql);
    char * errorMessage = 0;
    int errorCode = sqlite3_exec(db, sql.c_str(), callback, callArg, &errorMessage);
    if(errorCode != SQLITE_OK){
        logger.logError(std::string(errorMessage) + ". Query not executed.");
        throw QueryError(errorMessage);
    }
    logger.logInfo("Query finished succesfully.");
}

}
