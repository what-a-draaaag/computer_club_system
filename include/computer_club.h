#pragma once
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include "time_utils.h"
#include "event.h"

class ComputerClub {
public:
    ComputerClub(int n, const Time& open, const Time& close, int rate);

    void proc(const Event& e);
    std::vector<Event> get() const;
    std::vector<std::string> stats();
    void handleEnd();

private:
    int n;
    Time open;
    Time close;
    int rate;
    
    std::vector<Event> events;
    std::map<std::string, int> clientTable;
    std::map<int, std::string> tableClient;
    std::map<int, Time> tableStart;
    std::map<int, int> tableIncome;
    std::map<int, int> tableMins;
    std::queue<std::string> waitQueue;
    std::set<std::string> present;
    std::vector<bool> occupied;

    void handleArrival(const Event& e);
    void handleSitting(const Event& e);
    void handleWaiting(const Event& e);
    void handleLeaving(const Event& e);
    
    bool isTableBusy(int t) const;
    bool hasFreeTable() const;
    void assignTable(const Time tableTime, const std::string& name, int t);
    void freeTable(const std::string& n);
    void calcIncome(int t, const Time& end);
}; 