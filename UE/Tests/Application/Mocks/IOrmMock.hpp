#pragma once
#include <gmock/gmock.h>
#include "Database/Repository/IOrm.hpp"

namespace ue {

template <class T>
class IOrmMock : public IOrm<T>{
public:
    IOrmMock() = default;
    ~IOrmMock() = default;
    MOCK_METHOD(std::unique_ptr<T>, get, (int), (final));
    MOCK_METHOD(std::vector<T>, getAll, (), (final));
    MOCK_METHOD(T, insert, (T), (final));
    MOCK_METHOD(void, update, (T), (final));
    MOCK_METHOD(void, remove, (int), (final));
};

}
