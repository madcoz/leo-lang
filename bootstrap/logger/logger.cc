/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of logger */

#include <chrono>
#include <string>
#include "logger.hh"

using namespace std;

logger& logger::instance() {
    
    static logger self;
    return self;
}

string logger::get_now_time() {
    
    auto time_point = chrono::system_clock::now();
    time_t now = chrono::system_clock::to_time_t(time_point);
    string time_str = ctime(&now);
    return time_str.substr(0,time_str.length()-1); //remove the line feed from asctime
}

void logger::info(const std::string& s) {
    
    switch(setting) {
        case L2:
        case L3:
            *out << "[INFO] [" << get_now_time() << "] " << s << "\n";
        default:
            return;
    }
}
        
void logger::debug(const std::string& s) {
    
    switch(setting) {
        case L3:
            *out << "[DEBUG][" << get_now_time() << "] " << s << "\n";
        default:
            return;
    }
}
        
void logger::error(const std::string& s) {
    
    switch(setting) {
        case L0:
        case L1:
        case L2:
        case L3:
            *out << "[ERROR][" << get_now_time() << "] " << s << "\n";
        default:
            return;
    }
}
        
void logger::warn(const std::string& s) {
    
    switch(setting) {
        case L1:
        case L2:
        case L3:
            *out << "[WARN] [" << get_now_time() << "] " << s << "\n";
        default:
            return;
    }
}