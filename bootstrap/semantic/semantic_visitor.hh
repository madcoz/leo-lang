/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for semantic visitor */

#ifndef SEMANTIC_VISITOR_HH
#define SEMANTIC_VISITOR_HH

#include <stdexcept>

#include "../pattern/visitor.hh"
#include "../classtypetab/classtype/classtype.hh"
#include "../symtab/symbol/symbol.hh"

class semantic_visitor : public visitor {
    
    protected:
        void check_binary_action(const std::string& oper, class_type* cls_t, symbol* sym);
        
        void check_define_action(class_type* cls_t, symbol* sym);
        
        void check_define_int8(symbol* sym);
    
    public:
        void visit(ident_ast& node);
        
        void visit(ident_decl_ast& node);
        
        void visit(ident_def_ast& node);
        
        void visit(branch_ast& node);
        
        void visit(loop_ast& node);
        
        void visit(iter_loop_ast& node);
        
        void visit(binary_expr_ast& node);
        
        void visit(stmt_list_ast& node);
        
        void visit(global_expr_ast& node);
        
        void visit(unary_expr_ast& node);
        
        void visit(typecast_ast& node);
        
        void visit(num_ast& node);
        
        void visit(char_ast& node);
        
        void visit(string_ast& node);
        
        void visit(bool_ast& node);
        
        void visit(func_call_ast& node);
        
        void visit(func_decl_ast& node);
        
        void visit(func_def_ast& node);
        
        void visit(arg_ast& node);
        
        void visit(type_ast& node);
        
        void visit(struct_def_ast& node);
        
        void visit(struct_def_expr_ast& node);
        
        void visit(new_ast& node); 
};

class semantic_check_out_of_range : public std::out_of_range {
    
    public:
        explicit semantic_check_out_of_range(const std::string& what_arg)
            : std::out_of_range(what_arg) {}
            
        explicit semantic_check_out_of_range(const char* what_arg)
            : std::out_of_range(what_arg) {}
};

#endif