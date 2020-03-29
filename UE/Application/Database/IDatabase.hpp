#pragma once

#include <iostream>
#include <sqlite3.h>

namespace ue {

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual void executeQuery(std::string query,
                              int (*callback)(void *, int, char **, char **),
                              void * callArg) = 0;
protected:
    virtual void connect() = 0;
};

}
