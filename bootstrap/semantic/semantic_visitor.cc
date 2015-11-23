/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of semantic visitor */

#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <climits>
#include <cerrno>

#if defined(__CYGWIN__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 9)
#include "../stlx/string/string.hh"
using namespace stdx;
#endif

#include "semantic_visitor.hh"
#include "../symtab/symtab.hh"
#include "../logger/logger.hh"

using namespace std;

#define ASSERT_CAST(d,td,ts) \
    if(d == nullptr) { \
        LOGGER_ERROR("bug '" + string(ts) + "' cast to '" + string(td) + "' not possible"); \
        return; \
    }

//TO-DO: rearrange if need to separate to another class
string to_string(const yy::location& loc) {
    stringstream ss;
    ss << loc;
    return ss.str();
}    
    
void semantic_visitor::visit(ident_ast& node) {
    
    // If uid exists it means the identifier is already defined.
    if(node.get_uid().empty()) {
        LOGGER_ERROR(to_string(node.get_loc()) + ":undefine '" + node.get_name() + "' identifier");
    }
}

void semantic_visitor::visit(ident_decl_ast& node) {
    
    try {
        type_ast* type_expr = node.get_type_expr();
        type_expr->accept(*this);
        ident_ast* ident_expr = node.get_ident_expr();
        ident_expr->set_uid("<id>:" + ident_expr->get_name());
        symbol* sym = new ident_symbol(type_expr->release());
        sym->set_uid(ident_expr->get_uid());
        SYMTAB_INSERT(sym);
    } catch(const symtab_insert_invalid_argument& ex) {
        stringstream ss;
        ss << "redeclare '" + node.get_ident_expr()->get_name() + "' in current scope"  << "\n";
        LOGGER_ERROR(ss.str());
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(ident_def_ast& node) {
    
    //try {
        ident_decl_ast* ident_decl_expr = node.get_decl_expr();
        ident_decl_expr->accept(*this);
        ast* init_expr = node.get_init_expr();
        init_expr->accept(*this);
        //TO-DO: check compatible class types
    //}
}

void semantic_visitor::visit(branch_ast& node) {
    
    //try {
        ast* cond_expr = node.get_cond_expr();
        cond_expr->accept(*this);
        //TO-DO: check cond_expr is bool class type
        ast* true_stmt = node.get_true_stmt();
        SYMTAB_OPEN_SCOPE;
        true_stmt->accept(*this);
        SYMTAB_CLOSE_SCOPE;
        ast* false_stmt = node.get_false_stmt();
        if(false_stmt != nullptr) {
            SYMTAB_OPEN_SCOPE;
            false_stmt->accept(*this);
            SYMTAB_CLOSE_SCOPE;
        }
    //}
}

void semantic_visitor::visit(loop_ast& node) {
    
    //TO-DO: need to push continue, break label
    //       and pop when loop scope end
    //       that information is needed to check
    //       continue and break expression
    //try {
        SYMTAB_OPEN_SCOPE;
        ast* init_expr = node.get_init_expr();
        if(init_expr != nullptr) {
            init_expr->accept(*this);
        }
        ast* cond_expr = node.get_cond_expr();
        cond_expr->accept(*this);
        //TO-DO: check cond_expr is bool class type
        ast* rep_stmt = node.get_rep_stmt();
        if(rep_stmt != nullptr) {
            rep_stmt->accept(*this);
        }
        ast* inc_expr = node.get_inc_expr();
        if(inc_expr != nullptr) {
            inc_expr->accept(*this);
        }
        SYMTAB_CLOSE_SCOPE;
    //}
}

void semantic_visitor::visit(iter_loop_ast& node) {
    
    //TO-DO: need to push continue, break label
    //       and pop when loop scope end
    //       that information is needed to check
    //       continue and break expression
    try {
        SYMTAB_OPEN_SCOPE;
        ident_ast* array_ident_expr = node.get_array_ident_expr();
        array_ident_expr->accept(*this);
        symbol* tmp_sym = SYMTAB_LOOKUP(array_ident_expr->get_uid());
        ident_symbol* arr_id_sym = dynamic_cast<ident_symbol*>(tmp_sym);
        ASSERT_CAST(arr_id_sym, "ident_symbol*", "symbol*");
        //TO-DO: check arr_id_sym is array class type
        //       then we get duplicate elem class type
        ident_ast* elem_ident_expr = node.get_elem_ident_expr();
        elem_ident_expr->set_uid("<id>:" +  elem_ident_expr->get_name());
        class_type* el_cls_t = nullptr; //TO-DO: revise later
        symbol* el_id_sym = new ident_symbol(el_cls_t);
        el_id_sym->set_uid(elem_ident_expr->get_uid());
        SYMTAB_INSERT(el_id_sym);
        ast* rep_stmt = node.get_rep_stmt();
        if(rep_stmt != nullptr) {
            rep_stmt->accept(*this);
        }
        SYMTAB_CLOSE_SCOPE;
    } catch(const symtab_insert_invalid_argument& ex) {
        stringstream ss;
        ss << "redeclare '" + node.get_elem_ident_expr()->get_name() + "' in current scope"  << "\n";
        LOGGER_ERROR(ss.str());
        LOGGER_DEBUG(ex.what());
    } catch(const symtab_lookup_invalid_argument& ex) {
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(binary_expr_ast& node) {
    
    //try {
        string oper = node.get_oper();
        ast* lhs_expr = node.get_lhs_expr();
        lhs_expr->accept(*this);
        ast* rhs_expr = node.get_rhs_expr();
        rhs_expr->accept(*this);
        if(oper == "+") {
            //TO-DO: need implicit cast operations
        }
        //...
    //}
}

void semantic_visitor::visit(stmt_list_ast& node) {
    
    ast* curr_stmt = node.get_curr_stmt();
    curr_stmt->accept(*this);
    ast* next_stmt = node.get_next_stmt();
    if(next_stmt != nullptr) {
        next_stmt->accept(*this);
    }
}

void semantic_visitor::visit(global_expr_ast& node) {
    
    ast* include_expr = node.get_include_expr();
    ast* main_expr = node.get_main_expr();
    if(include_expr !=  nullptr) {
        //TO-DO: parse another file, mind cyclic inclusion,
        //       need to support module system
    }
    if(main_expr != nullptr) {
        SYMTAB_OPEN_SCOPE;
        main_expr->accept(*this);
        SYMTAB_CLOSE_SCOPE;
    }
}

void semantic_visitor::visit(unary_expr_ast& node) {
    
    //try {
        string oper = node.get_oper();
        ast* expr = node.get_expr();
        expr->accept(*this);
        if(oper == "+") {
            //TO-DO: need implicit cast operations
        }
        //...
    //}
}

void semantic_visitor::visit(typecast_ast& node) {
    
    //try {
        type_ast* type_expr = node.get_type_expr();
        type_expr->accept(*this);
        ast* expr = node.get_expr();
        expr->accept(*this);
        //TO-DO: check if casting possible
    //}
}

void semantic_visitor::visit(num_ast& node) {
    
    string num_str = node.get_num_str();
    if(num_str.find(".") != string::npos) {
        //TO-DO: support float and double type
        LOGGER_ERROR("not supported for float or double type for '" + num_str + "'" );
        return;
    } else {
        int base = 10;
        if(num_str.length() > 2 && num_str[0] == '0' && (num_str[1] == 'x' || num_str[1] == 'X')) {
            base = 16;
        } else if(num_str.length() > 1 && num_str[0] == '0') {
            base = 8;
        }
        try {
            symbol* sym = nullptr;
            //Use default uint32
            uint32_t ui32 = strtoul(num_str.c_str(), nullptr, base);
            if(ui32 == ULONG_MAX && errno == ERANGE) {
                //Try uint64
                errno = 0;
                uint64_t ui64 = strtoull(num_str.c_str(), nullptr, base);
                if(ui64 == ULLONG_MAX && errno == ERANGE) {
                    LOGGER_ERROR("literal number too large:" + num_str);
                    return;
                }
                node.set_uid("<ui64>:" + to_string(ui64));
                sym = new literal_uint64_symbol(ui64);
            } else {
                node.set_uid("<ui32>:" + to_string(ui32));
                sym = new literal_uint32_symbol(ui32);
            }
            if(sym == nullptr) {
                LOGGER_ERROR("bug sym is null");
                return;
            }
            sym->set_uid(node.get_uid());
            SYMTAB_INSERT(sym);
        } catch(const symtab_insert_invalid_argument& ex) {
            LOGGER_DEBUG("ignore literal number:" + num_str);
            LOGGER_DEBUG(ex.what());
        }
    }
}

void semantic_visitor::visit(char_ast& node) {
    
    string value = node.get_char_str();
    size_t length = value.length();
    if(length != 3 || (value[0] != '\'' && value[length-1] != '\'') ||
       (value[0] != '"' && value[length-1] != '"')) {
        LOGGER_ERROR("bug unknown literal char:" + value);
        return;
    }
    value = value.substr(1, length-2);
    try {
        node.set_uid("<c8>:" + value);
        symbol* sym = new literal_char_symbol(value);
        sym->set_uid(node.get_uid());
        SYMTAB_INSERT(sym);
    } catch(const symtab_insert_invalid_argument& ex) {
        LOGGER_DEBUG("ignore literal char:" + value);
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(string_ast& node) {
    
    string value = node.get_string_str();
    size_t length = value.length();
    if(length < 2 || (value[0] != '`' && value[length-1] != '`')){
        LOGGER_ERROR("bug unknown literal string:" + value);
        return;
    }
    if(length == 2) {
        value = "";
    } else {
        value = value.substr(1, length-2);
    }
    try {
        node.set_uid("<s>:" + value);
        symbol* sym = new literal_string_symbol(value);
        sym->set_uid(node.get_uid());
        SYMTAB_INSERT(sym);
    } catch(const symtab_insert_invalid_argument& ex) {
        LOGGER_DEBUG("ignore literal string:" + value);
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(bool_ast& node) {
    
    try {
        node.set_uid("<b>:" + (node.get_val()? std::string("true") : std::string("false")));
        symbol* sym = new literal_bool_symbol(node.get_val());
        sym->set_uid(node.get_uid());
        SYMTAB_INSERT(sym);
    } catch(const symtab_insert_invalid_argument& ex) {
        LOGGER_DEBUG("ignore literal bool:" + (node.get_val()? std::string("true") : std::string("false")));
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(func_call_ast& node) {

    //try {
        ast* func_expr = node.get_func_expr();
        func_expr->accept(*this);
        symbol* func_sym = SYMTAB_LOOKUP(func_expr->get_uid());
        if(func_sym->get_type() != SYMBOL_TYPE_IDENT) {
            LOGGER_ERROR("call function on rvalue");
            return;
        }
        ident_symbol* ident_sym = dynamic_cast<ident_symbol*>(func_sym);
        ASSERT_CAST(ident_sym, "ident_symbol*", "symbol*");
        class_type* cls_t = ident_sym->get_class_type();
        if(cls_t->get_type() != CLASS_TYPE_FUNC) {
            LOGGER_ERROR("call non-function type");
            return;
        }
        ast* paramlist = node.get_paramlist();
        if(paramlist != nullptr) {
            paramlist->accept(*this);
            //TO-DO: match paramlist type with func arglist type
        }
        //TO-DO: check access right
    //}
}

void semantic_visitor::visit(func_decl_ast& node) {
    
    try {
        ident_ast* ident_expr = node.get_ident_expr();
        ident_expr->set_uid("<id>:" + ident_expr->get_name());
        if(SYMTAB_EXIST_CURR(ident_expr->get_uid())) {
            LOGGER_ERROR("Redeclare '" + ident_expr->get_name() + "' in current scope");
            return;
        }
        type_ast* ret_type = node.get_ret_type();
        ret_type->accept(*this);
        ast* arglist = node.get_arglist();
        if(arglist !=  nullptr) {
            /*
            while(?)  {
                //TO-DO: check arglist duplicate name
            } 
            */
        }
        //TO-DO: check duplicate func signature
        //TO-DO: create func class type with the ret class type, func signature
        class_type* cls_t = nullptr; //TO-DO: revise later
        symbol* sym = new ident_symbol(cls_t);
        sym->set_uid(ident_expr->get_uid());
        SYMTAB_INSERT(sym);
    } catch(const symtab_insert_invalid_argument& ex) {
        LOGGER_ERROR("bug can't insert func symbol in current scope");
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(func_def_ast& node) {
    
    //TO-DO: push func for return expression validation
    //       pop when func scope ends
    //try {
        func_decl_ast* decl_expr = node.get_func_decl_expr();
        decl_expr->accept(*this);
        SYMTAB_OPEN_SCOPE;
        ast* arglist = decl_expr->get_arglist();
        if(arglist != nullptr) {
            /*
            while(?) {
                //TO-DO: insert the argument into the table
            }   
            */
        }
        node.get_impl_stmt()->accept(*this);
        SYMTAB_CLOSE_SCOPE;
    //}
}

void semantic_visitor::visit(arg_ast& node) {
   
    node.get_type_expr()->accept(*this);
}

void semantic_visitor::visit(type_ast& node) {
    
    //TO-DO: create class_type by type_name
    string type_name = node.get_type_name();
    class_type* cls_t = nullptr;
    if(type_name == CLASS_TYPE_INT8) {
        cls_t = new int8_class_type;
    }
    if(cls_t == nullptr) {
        LOGGER_ERROR(to_string(node.get_loc()) + ":type '" + type_name + "' is not defined");
        return;
    }
    node.store(cls_t);
}

void semantic_visitor::visit(struct_def_ast& node) {
    
    //TO-DO: push struct class type in order for struct_def_exp to define
    //       in the struct class type, pop when scope ends
    try {
        ident_ast* ident_expr = node.get_ident_expr();
        ident_expr->set_uid("<id>:" + ident_expr->get_name());
        if(SYMTAB_EXIST_CURR(ident_expr->get_uid())) {
            LOGGER_ERROR("Redefine '" + ident_expr->get_name() + "' in current scope");
            return;
        }
        class_type* cls_t = nullptr; //TO-DO: revise later
        symbol* sym = new ident_symbol(cls_t);
        sym->set_uid(ident_expr->get_uid());
        SYMTAB_INSERT(sym);
        SYMTAB_OPEN_SCOPE;
        node.get_impl_expr()->accept(*this);
        SYMTAB_CLOSE_SCOPE;
    } catch(const symtab_insert_invalid_argument& ex) {
        LOGGER_DEBUG(ex.what());
    }
}

void semantic_visitor::visit(struct_def_expr_ast& node) {
    
    //TO-DO: peek to get the struct class type
    //       save the definitions with the access type  
}

void semantic_visitor::visit(new_ast& node) {
    
    type_ast* type_expr = node.get_type_expr();
    //TO-DO: check if exists in class type table
    ast* arr_dimen_expr = node.get_arr_dimen_expr();
    if(arr_dimen_expr != nullptr) {
        //TO-DO: check if all array dimension expression is constants
        /*
        while(?) {
            
        }
        */
    }
}