/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for logger */

#ifndef LOGGER_HH
#define LOGGER_HH

#include <string>
#include <iostream>

#define LOGGER_INFO(s) (logger::instance().info(s))
#define LOGGER_DEBUG(s) (logger::instance().debug(s))
#define LOGGER_ERROR(s) (logger::instance().error(s))
#define LOGGER_WARN(s) (logger::instance().warn(s))
#define LOGGER_SET_LEVEL(s) (logger::instance().set_level(s))

class logger {
    
    public:
        enum level {
            L0, //error
            L1, //error, warn
            L2, //error, warn, info
            L3  //error, warn, info, debug
        };
    
    private:
        std::ostream* out;
        
        level setting;
        
        static std::string get_now_time();
    
    public:
        logger() : out(&std::cerr), setting(L0) {}
        
        virtual ~logger() { LOGGER_DEBUG("Cleaning logger"); }
    
        static logger& instance();
        
        void set_level(level setting) { this->setting = setting; }
        
        void set_out(std::ostream& out) { this->out = &out; }
        
        void info(const std::string& s);
        
        void debug(const std::string& s);
        
        void error(const std::string& s);
        
        void warn(const std::string& s);
        
        void reset() { setting = L0; out = &std::cerr;}
};

#endif