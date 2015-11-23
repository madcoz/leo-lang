/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for parsing driver */
#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include "../bison/parser.tab.hh"
#include "../ast/ast.hh"

#define YY_DECL \
    yy::leo_parser::symbol_type yylex(driver& drv)

YY_DECL;

class driver {
        
    public: 
        driver();
        
        virtual ~driver();
        
        void begin_scan();
        
        void end_scan();
        
        int parse(const std::string& fileName);
        
        void traverse(ast* node);
        
        void error(const yy::location& loc, const std::string& s);
        
        void error(const std::string& s);
        
        std::string filename;
        
        bool trace_scanning;
        
        bool trace_parsing;
};
#endif