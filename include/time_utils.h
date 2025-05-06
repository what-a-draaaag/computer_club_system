#pragma once
#include <string>
#include <stdexcept>

class Time {
public:
    Time(int hours = 0, int minutes = 0);
    Time(const std::string& timeStr);
    
    std::string toString() const;
    int toMinutes() const;
    bool operator<(const Time& other) const;
    
    void fromString(const std::string& timeStr);
    static bool isValidTimeString(const std::string& timeStr);

private:
    int hours;
    int minutes;
}; 