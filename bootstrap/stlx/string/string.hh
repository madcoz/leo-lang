/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for string library */

#include <string>

namespace stdx {
    
    std::string to_string(int val);
    
    std::string to_string(long val);
    
    std::string to_string(long long val);
    
    std::string to_string(unsigned val);
    
    std::string to_string(unsigned long val);
    
    std::string to_string(unsigned long long val);
    
    std::string to_string(float val);
    
    std::string to_string(double val);
    
    std::string to_string(long double val);
}