/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of the symbol table */

#include <iostream>
#include <utility>
#include <sstream>
#include <stdexcept>
#include "symtab.hh"

#include "../logger/logger.hh"

using namespace std;

symtab& symtab::instance() {
    
    static symtab self;
    return self;
}

string symtab::create_symbol_key(const string& uid, vector<scope>& scope_stack) {
    
    stringstream ss;
    ss << uid << ";<s>:" << create_scope_key(scope_stack);
    return ss.str();
}

string symtab::create_scope_key(vector<scope>& scope_stack) {
    
    stringstream ss;
    for(vector<scope>::iterator iter = scope_stack.begin(); iter != scope_stack.end(); ++iter) {
        ss << (*iter).get_value();
        if(iter+1 != scope_stack.end())
            ss << ".";
    }
    return ss.str();
}

void symtab::insert(symbol* s) {
    
    string symbol_key = create_symbol_key(s->get_uid(), this->scope_stack);
    
    if(table.count(symbol_key) > 0)  {
        throw symtab_insert_invalid_argument("duplicate key for " + symbol_key);
    }
    
    table.insert(make_pair<string, unique_ptr<symbol> >(symbol_key.c_str(), unique_ptr<symbol>(s)));
}

symbol* symtab::lookup(const string& uid) {
    
    symbol* search_symbol = nullptr;
    
    vector<scope> search_scope(this->scope_stack);
    
    stringstream ss;
    
    string symbol_key;
    
    while (!search_scope.empty()) {
        symbol_key = create_symbol_key(uid, search_scope);
        try {
            search_symbol = table.at(symbol_key).get();
            break;
        } catch(const out_of_range& oor) {
            ss << "out of range for " << uid << " caused by:" << oor.what() << "\n";
            LOGGER_DEBUG(ss.str());
            search_scope.pop_back();
        }
    }
    
    if(search_symbol == nullptr) {
        throw symtab_lookup_invalid_argument("can't find in all scope for " + uid);
    }
    
    return search_symbol;
}

bool symtab::exist(const string& uid, bool current) {
    
    symbol* search_symbol = nullptr;
    
    vector<scope> search_scope(this->scope_stack);
    
    stringstream ss;
    
    string symbol_key;
    
    while (!search_scope.empty()) {
        symbol_key = create_symbol_key(uid, search_scope);
        try {
            search_symbol = table.at(symbol_key).get();
            break;
        } catch(const out_of_range& oor) {
            ss << "out of range for " << uid << " caused by:" <<  oor.what() << "\n";
            LOGGER_DEBUG(ss.str());
            if(current) {
                break;
            }
            search_scope.pop_back();
        }
    }
    
    if(search_symbol == nullptr) {
        return false;
    }
    
    return true;
}

void symtab::open_scope() {
    
    scope_stack.push_back(scope::create(last));
    if(!last.empty()) {
        last.clear();
    }
}

void symtab::close_scope() {
    
    last = scope_stack.back();
    scope_stack.pop_back();
}

void symtab::clear() {
    
    LOGGER_DEBUG("cleaning symbol table");
    last.clear();
    scope_stack.clear();
}