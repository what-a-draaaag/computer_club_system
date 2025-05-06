#include "../include/event.h"
#include <sstream>

Event::Event(const Time& t, Type type, const std::string& name, int table)
    : t(t), type(type), name(name), table(table) {}

Event::Event(const Time& t, Type type, const std::string& name)
    : t(t), type(type), name(name) {}

Event::Event(const Time& t, Type type, Error err)
    : t(t), type(type), err(err) {}

Time Event::getT() const {
    return t;
}

Event::Type Event::getType() const {
    return type;
}

std::string Event::getName() const {
    return name;
}

int Event::getTable() const {
    return table;
}

Event::Error Event::getErr() const {
    return err;
}

std::string Event::getStr() const {
    std::stringstream ss;
    ss << t.toString() << " " << static_cast<int>(type);
    
    if (type == Type::ERROR) {
        if (err== Event::Error::NOT_OPEN)
            ss << " " << "NotOpenYet";
        if (err== Event::Error::PLACE_BUSY)
            ss << " " << "PlaceIsBusy";
        if (err== Event::Error::SHALL_NOT_PASS)
            ss << " " << "YouShallNotPass";
        if (err== Event::Error::UNKNOWN)
            ss << " " << "ClientUnknown";
        if (err== Event::Error::WAIT_NO_LONGER)
            ss << " " << "ICanWaitNoLonger!";
    } else if (type == Type::CLIENT_SAT || type == Type::CLIENT_SAT_FROM_QUEUE) {
        ss << " " << name << " " << table;
    } else {
        ss << " " << name;
    }
    
    return ss.str();
} 