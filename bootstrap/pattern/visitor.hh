/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for visitor pattern for ast nodes */

#ifndef VISITOR_HH
#define VISITOR_HH

#include "../ast/ast.hh"

class visitor {
    
    public:
        virtual void visit(ident_ast& node) = 0;
        
        virtual void visit(ident_decl_ast& node) = 0;
        
        virtual void visit(ident_def_ast& node) = 0;
        
        virtual void visit(branch_ast& node) = 0;
        
        virtual void visit(loop_ast& node) = 0;
        
        virtual void visit(iter_loop_ast& node) = 0;
        
        virtual void visit(binary_expr_ast& node) = 0;
        
        virtual void visit(stmt_list_ast& node) = 0;
        
        virtual void visit(global_expr_ast& node) = 0;
        
        virtual void visit(unary_expr_ast& node) = 0;
        
        virtual void visit(typecast_ast& node) = 0;
        
        virtual void visit(num_ast& node) = 0;
        
        virtual void visit(char_ast& node) = 0;
        
        virtual void visit(string_ast& node) = 0;
        
        virtual void visit(bool_ast& node) = 0;
        
        virtual void visit(func_call_ast& node) = 0;
        
        virtual void visit(func_decl_ast& node) = 0;
        
        virtual void visit(func_def_ast& node) = 0;
        
        virtual void visit(arg_ast& node) = 0;
        
        virtual void visit(type_ast& node) = 0;
        
        virtual void visit(struct_def_ast& node) = 0;
        
        virtual void visit(struct_def_expr_ast& node) = 0;
        
        virtual void visit(new_ast& node) = 0;
};

#endif