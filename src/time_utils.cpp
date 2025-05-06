#include "../include/time_utils.h"
#include <sstream>
#include <iomanip>

Time::Time(int hours, int minutes) : hours(hours), minutes(minutes) {
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        throw std::invalid_argument("Invalid time values");
    }
}

Time::Time(const std::string& timeStr) {
    this->fromString(timeStr);
}

std::string Time::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes;
    return ss.str();
}

int Time::toMinutes() const {
    return hours * 60 + minutes;
}

bool Time::operator<(const Time& other) const {
    return toMinutes() < other.toMinutes();
}


void Time::fromString(const std::string& timeStr) {
    if (!isValidTimeString(timeStr)) {
        throw std::invalid_argument("Invalid time format");
    }
    
    hours = std::stoi(timeStr.substr(0, 2));
    minutes = std::stoi(timeStr.substr(3, 2));
}

bool Time::isValidTimeString(const std::string& timeStr) {
    if (timeStr.length() != 5) return false;
    if (timeStr[2] != ':') return false;
    
    try {
        int hours = std::stoi(timeStr.substr(0, 2));
        int minutes = std::stoi(timeStr.substr(3, 2));
        return hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59;
    } catch (...) {
        return false;
    }
} 