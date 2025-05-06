#include "../include/computer_club.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

ComputerClub::ComputerClub(int n, const Time& open, const Time& close, int rate)
    : n(n), open(open), close(close), rate(rate) {
    occupied.resize(n, false);
}

void ComputerClub::proc(const Event& e) {
    events.push_back(e);
    
    switch (e.getType()) {
        case Event::Type::CLIENT_ARRIVED:
            handleArrival(e);
            break;
        case Event::Type::CLIENT_SAT:
            handleSitting(e);
            break;
        case Event::Type::CLIENT_WAITING:
            handleWaiting(e);
            break;
        case Event::Type::CLIENT_LEFT:
            handleLeaving(e);
            break;
        default:
            break;
    }
}


std::vector<Event> ComputerClub::get() const{
    return events;
}

std::vector<std::string> ComputerClub::stats() {
    std::vector<std::string> res;
    for (int i = 1; i <= n; ++i) {
        std::stringstream ss;
        Time tableTime(tableMins[i]/60, tableMins[i]%60);
        ss << i << " " << tableIncome[i] << " " << tableTime.toString();
        res.push_back(ss.str());
    }
    return res;
}

void ComputerClub::handleArrival(const Event& e) {
    const std::string& n = e.getName();
    
    if (present.find(n) != present.end()) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::SHALL_NOT_PASS));
        return;
    }
    
    if (e.getT() < open) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::NOT_OPEN));
        return;
    }
    
    present.insert(n);
}

void ComputerClub::handleSitting(const Event& e) {
    const std::string& n = e.getName();
    int t = e.getTable();
    
    if (present.find(n) == present.end()) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::UNKNOWN));
        return;
    }
    
    if (isTableBusy(t)) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::PLACE_BUSY));
        return;
    }
    
    if (clientTable.find(n) != clientTable.end()) {
        freeTable(n);
    }
    assignTable(e.getT(), n, t);
}

void ComputerClub::handleWaiting(const Event& e) {
    const std::string& client_name = e.getName();
    
    if (present.find(client_name) == present.end()) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::UNKNOWN));
        return;
    }
    
    if (hasFreeTable()) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::WAIT_NO_LONGER));
        return;
    }
    
    if (waitQueue.size() >= n) {
        present.erase(client_name);
        events.push_back(Event(e.getT(), Event::Type::CLIENT_FORCED_LEFT, client_name));
        return;
    }
    
    waitQueue.push(client_name);
}

void ComputerClub::handleLeaving(const Event& e) {
    const std::string& name = e.getName();
    
    if (present.find(name) == present.end()) {
        events.push_back(Event(e.getT(), Event::Type::ERROR, Event::Error::UNKNOWN));
        return;
    }
    
    if (clientTable.find(name) != clientTable.end()) {
        int t = clientTable[name];
        calcIncome(t, e.getT());
        freeTable(name);
        
        if (!waitQueue.empty()) {
            std::string next = waitQueue.front();
            waitQueue.pop();
            assignTable(e.getT(), next, t);
            events.push_back(Event(e.getT(), Event::Type::CLIENT_SAT_FROM_QUEUE, next, t));
        }
    }
    
    present.erase(name);
}

void ComputerClub::handleEnd() {
    std::vector<std::string> rem(present.begin(), present.end());
    std::sort(rem.begin(), rem.end());
    
    for (const auto& name : rem) {
        if (clientTable.find(name) != clientTable.end()) {
            int t = clientTable[name];
            calcIncome(t, close);
        }
        events.push_back(Event(close, Event::Type::CLIENT_FORCED_LEFT, name));
    }
}

bool ComputerClub::isTableBusy(int t) const {
    return tableClient.find(t) != tableClient.end();
}

bool ComputerClub::hasFreeTable() const {
    return tableClient.size() < n;
}

void ComputerClub::assignTable(const Time tableTime, const std::string& name, int t) {
    clientTable[name] = t;
    tableClient[t] = name;
    tableStart[t] = tableTime;
}

void ComputerClub::freeTable(const std::string& name) {
    int t = clientTable[name];
    clientTable.erase(name);
    tableClient.erase(t);
    tableStart.erase(t);
}

void ComputerClub::calcIncome(int t, const Time& end) {
    Time s = tableStart[t];
    int m = end.toMinutes() - s.toMinutes();
    int h = (m + 59) / 60;
    
    tableIncome[t] += h * rate;
    tableMins[t] += m;
} 
