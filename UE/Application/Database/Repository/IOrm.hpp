#pragma once

#include <memory>
#include "Logger/PrefixedLogger.hpp"


namespace ue {

template <class T>
class IOrm{
public:
    virtual ~IOrm() = default;
    virtual std::unique_ptr<T> getById(int) = 0;
    virtual std::vector<T> getAll() = 0;
    virtual T insert(T elem) = 0;
    virtual void update(T elem) = 0;
    virtual void remove(int id) = 0;
};

}
