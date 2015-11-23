/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of the parser driver */

#include <sstream>
#include "driver.hh"
#include "../semantic/semantic_visitor.hh"
#include "../logger/logger.hh"
#include "../symtab/symtab.hh"

#if defined(__CYGWIN__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 9)
#include "../stlx/string/string.hh"
using namespace stdx;
#endif

using namespace std;

driver::driver()
    :trace_scanning(false), trace_parsing(false) {
}

driver::~driver() {
}

int driver::parse(const string& filename) {
    
    LOGGER_INFO("Source filename:" + filename);
    this->filename = filename;
    SYMTAB_CLEAR;
    LOGGER_INFO("Start scanner");
    begin_scan();
    LOGGER_INFO("Start parsing");
    yy::leo_parser parser(*this);
    LOGGER_DEBUG("Trace syntax:" + string(trace_parsing? "true" : "false"));
    parser.set_debug_level(trace_parsing);
    int result = parser.parse();
    LOGGER_DEBUG("Parse result:" + to_string(result));
    end_scan();
    LOGGER_INFO("Stop scanner");
    return result;
}

void driver::traverse(ast* node) {
    semantic_visitor semantic_check;
    node->accept(semantic_check);
    //node->accept(codegen_llvm_visitor);
}

void driver::error(const yy::location& loc, const string& s) {
    
    std::stringstream ss;
    ss << loc <<  ":" << s;
    error(ss.str());
}

void driver::error(const string& s) {
    
    LOGGER_ERROR(s);
}