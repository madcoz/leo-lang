/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for semantic visitor */

#ifndef SEMANTIC_VISITOR_HH
#define SEMANTIC_VISITOR_HH

#include <stdexcept>
#include <functional>
#include <utility>
#include <unordered_map>

#include "../pattern/visitor.hh"
#include "../classtypetab/classtype/classtype.hh"
#include "../symtab/symbol/symbol.hh"

class semantic_visitor : public visitor {
    
    protected:
        std::unordered_map<std::string,
                           std::pair<std::function<bool(uint32_t)>, 
                           std::function<bool(uint64_t)> > > def_num_range_val_map;
    
        void check_binary_action(const std::string& oper, class_type* cls_t, symbol* sym);
        
        void check_define_action(class_type* cls_t, symbol* sym);
        
        void check_define_numeric(const std::string& type_str, 
                                  std::pair<std::function<bool(uint32_t)>, 
                                            std::function<bool(uint64_t)> > check_out_of_range_pair, 
                                  symbol* sym);
    
    public:
        semantic_visitor();
        
        virtual ~semantic_visitor() { def_num_range_val_map.clear(); }
    
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

class semantic_check_unsupported_type : public std::invalid_argument {

    public:
        explicit semantic_check_unsupported_type(const std::string& what_arg)
            : std::invalid_argument(what_arg) {}
            
        explicit semantic_check_unsupported_type(const char* what_arg)
            : std::invalid_argument(what_arg) {}
        
};

#endif