/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for symbol table */

#ifndef SYMTAB_HH
#define SYMTAB_HH

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include "scope.hh"
#include "symbol/symbol.hh"

#define SYMTAB_LOOKUP(name) (symtab::instance().lookup(name))
#define SYMTAB_INSERT(sym) (symtab::instance().insert(sym))
#define SYMTAB_EXIST_CURR(sym) (symtab::instance().exist(sym, true))
#define SYMTAB_EXIST_ALL(sym) (symtab::instance().exist(sym, false))
#define SYMTAB_OPEN_SCOPE (symtab::instance().open_scope())
#define SYMTAB_CLOSE_SCOPE (symtab::instance().close_scope())
#define SYMTAB_CLEAR (symtab::instance().clear())

class symtab  {
    
    private:
        std::unordered_map<std::string, std::unique_ptr<symbol>> table;
        
        scope last;
        
        std::vector<scope> scope_stack;
        
        static std::string create_symbol_key(const std::string& uid, std::vector<scope>& scope_stack);
        
        static std::string create_scope_key(std::vector<scope>& scope_stack);
        
    public:
        symtab() {}
        
        virtual ~symtab() { clear(); }
        
        static symtab& instance();
        
        void insert(symbol* s);
        
        symbol* lookup(const std::string& uid);
        
        bool exist(const std::string& name, bool current);
        
        void open_scope();
        
        void close_scope();
        
        void clear();
    
};

class symtab_insert_invalid_argument : public std::invalid_argument {
    
    public:
        explicit symtab_insert_invalid_argument(const std::string& what_arg)
            : std::invalid_argument(what_arg) {}
            
        explicit symtab_insert_invalid_argument(const char* what_arg)
            : std::invalid_argument(what_arg) {}
};

class symtab_lookup_invalid_argument : public std::invalid_argument {
    
    public:
        explicit symtab_lookup_invalid_argument(const std::string& what_arg)
            : std::invalid_argument(what_arg) {}
        
        explicit symtab_lookup_invalid_argument(const char* what_arg)
            : std::invalid_argument(what_arg) {}
};

#endif