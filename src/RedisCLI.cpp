#include "RedisCLI.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

void RedisCLI::set(const std::string& key, const std::string& value) {
    db.put(key, value);
}

std::string RedisCLI::get(const std::string& key) {
    try {
        return db.get(key);
    } catch (const std::out_of_range&) {
        return "(nil)";
    }
}

bool RedisCLI::del(const std::string& key) {
    if (db.contains(key)) {
        db.remove(key);
        return true;
    }
    return false;
}

bool RedisCLI::exists(const std::string& key) const {
    return db.contains(key);
}

int RedisCLI::count() const {
    return db.size();
}

void RedisCLI::clear() {
    db.clear();
}

void RedisCLI::run() {
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "EXIT") {
            break;
        } else if (cmd == "SET") {
            std::string key, value;
            if (iss >> key >> value) {
                set(key, value);
                std::cout << "OK\n";
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'SET' command\n";
            }
        } else if (cmd == "GET") {
            std::string key;
            if (iss >> key) {
                std::cout << get(key) << "\n";
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'GET' command\n";
            }
        } else if (cmd == "DEL") {
            std::string key;
            if (iss >> key) {
                std::cout << "(integer) " << (del(key) ? 1 : 0) << "\n";
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'DEL' command\n";
            }
        } else if (cmd == "EXISTS") {
            std::string key;
            if (iss >> key) {
                std::cout << "(integer) " << (exists(key) ? 1 : 0) << "\n";
            } else {
                std::cout << "(error) ERR wrong number of arguments for 'EXISTS' command\n";
            }
        } else if (cmd == "COUNT") {
            std::cout << "(integer) " << count() << "\n";
        } else if (cmd == "CLEAR") {
            clear();
            std::cout << "OK\n";
        } else {
            std::cout << "(error) ERR unknown command '" << cmd << "'\n";
        }
    }
}
