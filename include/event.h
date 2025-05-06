#pragma once
#include <string>
#include "time_utils.h"

class Event {
public:
    enum class Type {
        CLIENT_ARRIVED = 1,
        CLIENT_SAT = 2,
        CLIENT_WAITING = 3,
        CLIENT_LEFT = 4,
        CLIENT_FORCED_LEFT = 11,
        CLIENT_SAT_FROM_QUEUE = 12,
        ERROR = 13
    };

    enum class Error{
        NOT_OPEN,
        PLACE_BUSY,
        WAIT_NO_LONGER,
        UNKNOWN,
        SHALL_NOT_PASS
    };

    Event(const Time& t, Type type, const std::string& name, int table);
    Event(const Time& t, Type type, const std::string& name);
    Event(const Time& t, Type type, Error err);

    Time getT() const;
    Type getType() const;
    std::string getName() const;
    int getTable() const;
    Error getErr() const;
    std::string getStr() const;

private:
    Time t;
    Type type;
    std::string name;
    int table;
    Error err;
}; 