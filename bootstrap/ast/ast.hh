/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Declarations for abstract syntax tree */

#ifndef AST_H
#define AST_H

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <utility>

#include "../bison/location.hh"
#include "../classtypetab/classtype/classtype.hh"

class visitor;

class ast {
    
    private:
        std::string uid;
    
    public:
        virtual ~ast(){}
        
        virtual void accept(visitor& v) = 0;
        
        virtual std::ostream& ostream(std::ostream& o) { return o; }
        
        virtual void set_uid(const std::string& uid) { this->uid = uid; }
        
        virtual std::string get_uid() { return uid; }
        
};

class ident_ast : public ast {
    
    private:
        std::string name;
        
        yy::location loc; //TO-DO: mind that the filename is not copied
        
    public:
        ident_ast(const std::string& name,const yy::location& loc);
        
        void accept(visitor& v);
        
        std::string get_name() { return name; }
        
        const yy::location& get_loc() { return loc; }
            
        virtual std::ostream& ostream(std::ostream& o) { 
        
            return o << "ident name:" << name; 
        }
};

class type_ast : public ast {
    
    private:
        std::string type_name;
        
        yy::location loc; //TO-DO: mind that the filename is not copied
        
        int dimen;
        
        bool is_struct;
        
        std::unique_ptr<class_type> cls_t;
        
    public:
        type_ast(const std::string& type_name, const yy::location& loc, int dimen= 0, bool is_struct = false);
        
        virtual ~type_ast() { cls_t.reset(); }
        
        void accept(visitor& v);
        
        bool if_is_struct() { return is_struct; }
        
        int get_dimen() { return dimen; }
        
        std::string get_type_name() { return type_name; }
        
        void store(class_type* cls_t) { this->cls_t.reset(cls_t); }
        
        class_type* release() { return cls_t.release(); }
        
        const yy::location& get_loc() { return loc; }
            
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "type name:" << type_name 
              << ",dimen:" <<  dimen
              << ",is struct:" << (is_struct? "true" : "false");
            return o;
        }
};

class ident_decl_ast : public ast {
    
    private:
        std::unique_ptr<ident_ast> ident_expr;
        
        std::unique_ptr<type_ast> type_expr;
        
    public:
        ident_decl_ast(ident_ast* ident_expr, type_ast* type_expr);
              
        virtual ~ident_decl_ast() { ident_expr.reset(); type_expr.reset(); }
        
        void accept(visitor& v);
        
        ident_ast* get_ident_expr() { return ident_expr.get(); }
        
        type_ast* get_type_expr() { return type_expr.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "decl expr ";
            ident_expr->ostream(o);
            o << ",";
            type_expr->ostream(o);
            return o;
        }
};

class ident_def_ast : public ast {
    
    private:
        std::unique_ptr<ident_decl_ast> decl_expr;
        
        std::unique_ptr<ast> init_expr;
        
    public:
        ident_def_ast(ident_decl_ast* decl_expr, ast* init_expr);
              
        virtual ~ident_def_ast() { decl_expr.reset(); init_expr.reset(); }
        
        void accept(visitor& v);
        
        ident_decl_ast* get_decl_expr() { return decl_expr.get(); }
        
        ast* get_init_expr() { return init_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            decl_expr->ostream(o);
            o << ",init expr:" ;
            init_expr == nullptr? o << "<default>" : init_expr->ostream(o);
            return o;
        }
};

class branch_ast : public ast {
    
    private:
        std::unique_ptr<ast> cond_expr, true_stmt, false_stmt;
    
    public:
        branch_ast(ast* cond_expr,
                   ast* true_stmt)
            : branch_ast(cond_expr, true_stmt, nullptr) {}
              
        branch_ast(ast* cond_expr, ast* true_stmt, ast* false_stmt);
              
        virtual ~branch_ast() { cond_expr.reset(); true_stmt.reset(); false_stmt.reset(); }
        
        void accept(visitor& v);
        
        ast* get_cond_expr() { return cond_expr.get(); }
        
        ast* get_true_stmt() { return true_stmt.get(); }
        
        ast* get_false_stmt() { return false_stmt.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "cond expr:";
            cond_expr->ostream(o);
            o << ",true stmt:";
            true_stmt == nullptr? o << "<none>" : true_stmt->ostream(o);
            o << ",false stmt:";
            false_stmt == nullptr? o << "<none>" : false_stmt->ostream(o);
            return o;
        }
};

class loop_ast : public ast {
    
    private:
        std::unique_ptr<ast> init_expr, cond_expr, rep_stmt, inc_expr;
        
    public:
        loop_ast(ast* init_expr,
                 ast* cond_expr,
                 ast* rep_stmt,
                 ast* inc_expr);
              
        virtual ~loop_ast() { init_expr.reset(); cond_expr.reset(); rep_stmt.reset(); inc_expr.reset(); }
        
        void accept(visitor& v);
        
        //std::string get_signature() { return "?"; } //TO-DO:support signature
        
        ast* get_init_expr() { return init_expr.get(); }
        
        ast* get_cond_expr() { return cond_expr.get(); }
        
        ast* get_rep_stmt() { return rep_stmt.get(); }
        
        ast* get_inc_expr() { return inc_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "init expr:";
            init_expr == nullptr? o << "<none>" : init_expr->ostream(o);
            o << ",cond expr:";
            cond_expr == nullptr? o << "<true>" : cond_expr->ostream(o);
            o << ",rep stmt:";
            rep_stmt == nullptr? o << "<none>" : rep_stmt->ostream(o);
            o << ",inc expr:";
            inc_expr == nullptr? o << "<none>" : inc_expr->ostream(o);
            return o;
        }
};

class iter_loop_ast : public ast {
    
    private:
        std::unique_ptr<ident_ast> elem_ident_expr, array_ident_expr;
        
        std::unique_ptr<ast> rep_stmt;
        
    public:
        iter_loop_ast(ident_ast* elem_ident_expr,
                 ident_ast* array_ident_expr,
                 ast* rep_stmt);
              
        virtual ~iter_loop_ast() { elem_ident_expr.reset(); array_ident_expr.reset(); rep_stmt.reset(); }
        
        void accept(visitor& v);
        
        ident_ast* get_elem_ident_expr() { return elem_ident_expr.get(); }
        
        ident_ast* get_array_ident_expr() { return array_ident_expr.get(); }
        
        ast* get_rep_stmt() { return rep_stmt.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "elem ";
            elem_ident_expr->ostream(o);
            o << ",array ";
            array_ident_expr->ostream(o);
            o << ",rep stmt:";
            rep_stmt == nullptr? o << "<none>" : rep_stmt->ostream(o);
            return o;
        }
};

class binary_expr_ast : public ast {
    
    private:
        std::string oper;
        
        std::unique_ptr<ast> lhs_expr, rhs_expr;
        
    public:
        binary_expr_ast(const std::string& oper, ast* lhs_expr, ast* rhs_expr);
              
        virtual ~binary_expr_ast() { lhs_expr.reset(); rhs_expr.reset(); }
        
        void accept(visitor& v);
        
        std::string get_oper() { return oper; }
        
        ast* get_lhs_expr() { return lhs_expr.get(); }
        
        ast* get_rhs_expr() { return rhs_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "oper:" << oper
              << ",lhs expr:";
            lhs_expr->ostream(o);
            o << ",rhs expr:";
            rhs_expr->ostream(o);
            return o;
        }
};

class stmt_list_ast : public ast {
    
    private:
        std::unique_ptr<ast> curr_stmt, next_stmt;
        
    public:
        stmt_list_ast(ast* curr_stmt, ast* next_stmt);
        
        virtual ~stmt_list_ast() { curr_stmt.reset(); next_stmt.reset(); }
        
        void accept(visitor& v);
        
        ast* get_curr_stmt() { return curr_stmt.get(); }
        
        ast* get_next_stmt() { return next_stmt.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "curr stmt:";
            curr_stmt->ostream(o);
            o << "next stmt:";
            next_stmt == nullptr? o << "<none>" : next_stmt->ostream(o); 
            return o;
        }
};

class global_expr_ast : public ast {
    
    private:
        std::unique_ptr<ast> include_expr, main_expr;
        
    public:
        global_expr_ast(ast* include_expr, ast* main_expr);
        
        virtual ~global_expr_ast() { include_expr.reset(); main_expr.reset(); }
        
        void accept(visitor& v);
        
        ast* get_include_expr() { return include_expr.get(); }
        
        ast* get_main_expr() { return main_expr.get(); }
        
        std::ostream& ostream(std::ostream& o) {
            
            o << "include expr:";
            include_expr == nullptr? o << "<none>" : include_expr->ostream(o);
            o << "main expr:";
            main_expr == nullptr? o << "<none>" : main_expr->ostream(o);
            return o;
        }
};

class unary_expr_ast : public ast {
    
    private:
        std::string oper;
        
        std::unique_ptr<ast> expr;
    
    public:
        unary_expr_ast(const std::string& oper, ast* expr);
        
        virtual ~unary_expr_ast() { expr.reset(); }
        
        void accept(visitor& v);
        
        std::string get_oper() {  return oper; }
        
        ast* get_expr() { return expr.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "oper:" << oper
              << ",expr:";
            expr == nullptr? o << "<none>" : expr->ostream(o);
            return o;
        }
};

class typecast_ast : public ast {
    
    private:
        std::unique_ptr<type_ast> type_expr;
        
        std::unique_ptr<ast> expr;
    
    public:
        typecast_ast(type_ast* type_expr, ast* expr);
              
        virtual ~typecast_ast() { type_expr.reset(); expr.reset(); }

        void accept(visitor& v);
        
        type_ast* get_type_expr() { return type_expr.get(); }
        
        ast* get_expr() { return expr.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "typecast:";
            type_expr->ostream(o);
            o  << ",expr:";
            expr->ostream(o);
            return o;
        }
};

class num_ast : public ast {
    
    private:
        std::string num_str;
        
    public:
        num_ast(const std::string& num_str);
        
        void accept(visitor& v);
        
        std::string get_num_str() { return num_str; }
            
        virtual std::ostream& ostream(std::ostream& o) {
            
            return o << "num:" << num_str;
        }
};

class char_ast : public ast {
    
    private:
        std::string char_str;
    
    public:
        char_ast(const std::string& char_str);
        
        void accept(visitor& v);
        
        std::string get_char_str() { return char_str; }
            
        virtual std::ostream& ostream(std::ostream& o) {
            
            return o << "char:" << char_str;
        }
};

class string_ast : public ast {
    
    private:
        std::string string_str;
        
    public:
        string_ast(const std::string& string_str);
        
        void accept(visitor& v);
        
        std::string get_string_str() { return string_str; }
            
        virtual std::ostream& ostream(std::ostream& o) {
            
            return o << "string:" << string_str;
        }
};

class bool_ast : public ast {
    
    private:
        bool val;
        
    public:
        bool_ast(const bool val);
        
        void accept(visitor& v);
        
        bool get_val() { return val; }
            
        virtual std::ostream& ostream(std::ostream& o) {
            
            return o << "bool:" << val;
        }
};

class func_call_ast : public ast {
    
    private:
        std::unique_ptr<ast> func_expr, paramlist ;
    
    public:
        func_call_ast(ast* func_expr, ast* paramlist);
              
        virtual ~func_call_ast() { func_expr.reset(); paramlist.reset(); }
        
        void accept(visitor& v);
        
        ast* get_func_expr()  { return func_expr.get(); }
        
        ast* get_paramlist() { return paramlist.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "call func expr:";
            func_expr->ostream(o);
            o << ",paramlist:";
            paramlist == nullptr? o << "<none>" : paramlist->ostream(o);
            return o;
        }
};

class func_decl_ast : public ast {
    
    private:
        std::unique_ptr<ident_ast> ident_expr;
        
        std::unique_ptr<type_ast> ret_type;
        
        std::unique_ptr<ast> arglist;
        
    public:
        func_decl_ast(ident_ast* ident_expr, type_ast* ret_type, ast* arglist);
              
        virtual ~func_decl_ast() { ident_expr.reset(); ret_type.reset(); arglist.reset(); }
        
        void accept(visitor& v);
        
        ident_ast* get_ident_expr() { return ident_expr.get(); }
        
        ast* get_arglist() { return arglist.get(); }
        
        type_ast* get_ret_type() { return ret_type.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "decl func ";
            ident_expr->ostream(o);
            o << ",ret ";
            ret_type == nullptr? o << ":<none>" : ret_type->ostream(o);
            o << ",arglist:";
            arglist == nullptr? o << "<none>" : arglist->ostream(o);
            return o;
        }
};

class func_def_ast : public ast {
    
    private:
        std::unique_ptr<func_decl_ast> decl_expr;
        
        std::unique_ptr<ast> impl_stmt;
    
    public:
        func_def_ast(func_decl_ast* decl_expr, ast* impl_stmt);
              
        virtual ~func_def_ast() { decl_expr.reset(); impl_stmt.reset(); }
        
        void accept(visitor& v);
        
        func_decl_ast* get_func_decl_expr() { return decl_expr.get(); }
        
        ast* get_impl_stmt() { return impl_stmt.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            decl_expr->ostream(o);
            o << ",impl stmt:";
            impl_stmt == nullptr? o << "<none>" : impl_stmt->ostream(o);
            return o;
        }
};

class arg_ast : public ast {
    
    private:
        std::unique_ptr<ident_ast> ident_expr;
        
        std::unique_ptr<type_ast> type_expr;
        
    public:
        arg_ast(ident_ast* ident_expr, type_ast* type_expr);
              
        virtual ~arg_ast() { ident_expr.reset(); type_expr.reset(); }
        
        void accept(visitor& v);
        
        ident_ast* get_ident_expr() { return ident_expr.get(); }
        
        type_ast* get_type_expr() { return type_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "arg ";
            ident_expr->ostream(o);
            o << ",type:";
            type_expr->ostream(o);
            return o;
        }
};

class struct_def_ast : public ast {
    
    private:
        std::unique_ptr<ident_ast> ident_expr;
        
        std::unique_ptr<ast> impl_expr;
        
    public:
        struct_def_ast(ident_ast* ident_expr, ast* impl_expr);
              
        virtual ~struct_def_ast() { ident_expr.reset(); impl_expr.reset(); }
        
        void accept(visitor& v);
        
        ident_ast* get_ident_expr() { return ident_expr.get(); }
        
        ast* get_impl_expr() { return impl_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "def ";
            ident_expr->ostream(o);
            o << ",impl expr:";
            impl_expr == nullptr? o << "<none>" : impl_expr->ostream(o);
            return o;
        }
};

class struct_def_expr_ast : public ast {
    
    private:
        std::string access;
        
        std::unique_ptr<ast> def_expr;
    
    public:
        struct_def_expr_ast(const std::string& access, ast* def_expr);
              
        virtual ~struct_def_expr_ast() { def_expr.reset(); }
        
        void accept(visitor& v);
        
        std::string get_access() { return access; }
        
        ast* get_def_expr() { return def_expr.get(); }
        
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "access:" << access
              << ",def expr:";
            def_expr == nullptr? o << "<none>" : def_expr->ostream(o);
            return o;
        }
};

class new_ast : public ast {
    
    private:
        std::unique_ptr<type_ast> type_expr;
        std::unique_ptr<ast> arr_dimen_expr;
        
    public:
        new_ast(type_ast* type_expr, ast* arr_dimen_expr);
              
        virtual ~new_ast() { type_expr.reset(); arr_dimen_expr.reset(); }
        
        void accept(visitor& v);
        
        type_ast* get_type_expr() { return type_expr.get(); }
        
        ast* get_arr_dimen_expr() { return arr_dimen_expr.get(); }
              
        virtual std::ostream& ostream(std::ostream& o) {
            
            o << "new ";
            type_expr->ostream(o);
            o << ",arr dimen expr:";
            arr_dimen_expr == nullptr? o << "<none>" : arr_dimen_expr->ostream(o);
            return o;
        }
};

#endif