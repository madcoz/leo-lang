/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of string library */

#include "string.hh"
#include <sstream>

using namespace std;

#define RET_STDX_TO_STRING(val) \
        stringstream ss; \
        ss << val; \
        return ss.str();

string stdx::to_string(int val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(long val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(long long val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(unsigned val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(unsigned long val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(unsigned long long val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(float val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(double val) { RET_STDX_TO_STRING(val); }
    
string stdx::to_string(long double val) { RET_STDX_TO_STRING(val); }