/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for scope */

#ifndef SCOPE_HH
#define SCOPE_HH

class scope {
    
    private:
        int value;
        
    public:
        scope() { clear(); };
        
        scope(const scope& other) : value(other.value) {}
            
        void clear() { value = -1; }
        
        bool empty() { return value == -1; }
        
        int get_value() { return value; }
        
        static scope create(scope& scope_ref);
};

#endif