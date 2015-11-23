/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of the scope */

#include "scope.hh"

scope scope::create(scope& scope_ref) {
    
    scope temp(scope_ref);
    ++(temp.value);
    return temp;
}