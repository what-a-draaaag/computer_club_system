#include "../include/computer_club.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool readConfig(std::ifstream& in, int& n, Time& open, Time& close, int& rate) {
    std::string line;
    if (!std::getline(in, line)) return false;
    std::istringstream iss(line);
    if (!(iss >> n) || n <= 0) return false;
    
    if (!std::getline(in, line)) return false;
    iss.clear();
    iss.str(line);
    std::string openStr, closeStr;
    if (!(iss >> openStr >> closeStr)) return false;
    open.fromString(openStr);
    close.fromString(closeStr);
    
    if (!std::getline(in, line)) return false;
    iss.clear();
    iss.str(line);
    if (!(iss >> rate) || rate <= 0) return false;
    
    return true;
}

Event readEvent(const std::string& line) {
    std::istringstream iss(line);
    std::string timeStr;
    int type;
    std::string name;
    int table = 0;
    
    if (!(iss >> timeStr >> type >> name)) {
        throw std::runtime_error("invalid format");
    }
    
    Time t;
    t.fromString(timeStr);
    
    switch (static_cast<Event::Type>(type)) {
        case Event::Type::CLIENT_ARRIVED:
            return Event(t, static_cast<Event::Type>(type), name);

        case Event::Type::CLIENT_WAITING:
            return Event(t, static_cast<Event::Type>(type), name);

        case Event::Type::CLIENT_LEFT:
            return Event(t, static_cast<Event::Type>(type), name);
            
        case Event::Type::CLIENT_SAT:
            if (!(iss >> table)) {
                throw std::runtime_error("invalid format: number of the table required");
            }
            return Event(t, static_cast<Event::Type>(type), name, table);
            
        default:
            throw std::runtime_error("invalid type");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "example of usage:" << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "file can't be opened" << std::endl;
        return 1;
    }
    
    try {
        int n;
        Time open, close;
        int rate;
        
        if (!readConfig(in, n, open, close, rate)) {
            std::cerr << "invalid data about computer club" << std::endl;
            return 1;
        }
        
        ComputerClub club(n, open, close, rate);
        std::string line;
        
        std::cout << open.toString() << std::endl;
        
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            
            try {
                Event e = readEvent(line);
                club.proc(e);
            } catch (const std::exception& e) {
                std::cerr << "error in proessing events: " << e.what() << std::endl;
                return 1;
            }
        }

        club.handleEnd();
        std::vector<Event> ev = club.get();
        for (Event e: ev){
            std::cout << e.getStr() << std::endl;
        }
        
        std::cout << close.toString() << std::endl;
        for (const auto& stat : club.stats()) {
            std::cout << stat << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 
