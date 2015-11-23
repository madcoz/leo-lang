/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for semantic visitor */

#ifndef SEMANTIC_VISITOR_HH
#define SEMANTIC_VISITOR_HH

#include "../pattern/visitor.hh"

class semantic_visitor : public visitor {
    
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

#endif