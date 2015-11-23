/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Implementation of the abstract syntax tree */

#include "ast.hh"
#include "../pattern/visitor.hh"

using namespace std;

ident_ast::ident_ast(const string& name,const yy::location& loc) 
    : name(name), loc(loc) {}
    
void ident_ast::accept(visitor& v) { v.visit(*this); }

ident_decl_ast::ident_decl_ast(ident_ast* ident_expr, type_ast* type_expr)
    : ident_expr(ident_expr), type_expr(type_expr) {}
    
void ident_decl_ast::accept(visitor& v) { v.visit(*this); }

ident_def_ast::ident_def_ast(ident_decl_ast* decl_expr, ast* init_expr)
    : decl_expr(decl_expr), init_expr(init_expr) {}
    
void ident_def_ast::accept(visitor& v) { v.visit(*this); }

branch_ast::branch_ast(ast* cond_expr, ast* true_stmt, ast* false_stmt)
    : cond_expr(cond_expr), true_stmt(true_stmt), false_stmt(false_stmt) {}
    
void branch_ast::accept(visitor& v) { v.visit(*this); }

loop_ast::loop_ast(ast* init_expr,
                 ast* cond_expr,
                 ast* rep_stmt,
                 ast* inc_expr)
    : init_expr(init_expr), cond_expr(cond_expr), rep_stmt(rep_stmt), inc_expr(inc_expr) {}
    
void loop_ast::accept(visitor& v) { v.visit(*this); }

iter_loop_ast::iter_loop_ast(ident_ast* elem_ident_expr,
                 ident_ast* array_ident_expr,
                 ast* rep_stmt)
    : elem_ident_expr(elem_ident_expr), array_ident_expr(array_ident_expr), rep_stmt(rep_stmt) {}
    
void iter_loop_ast::accept(visitor& v) { v.visit(*this); }

binary_expr_ast::binary_expr_ast(const std::string& oper, ast* lhs_expr, ast* rhs_expr)
    : oper(oper), lhs_expr(lhs_expr), rhs_expr(rhs_expr) {}
    
void binary_expr_ast::accept(visitor& v) { v.visit(*this); }

stmt_list_ast::stmt_list_ast(ast* curr_stmt, ast* next_stmt)
    : curr_stmt(curr_stmt), next_stmt(next_stmt) {}
    
void stmt_list_ast::accept(visitor& v) { v.visit(*this); }
    
global_expr_ast::global_expr_ast(ast* include_expr, ast* main_expr)
    : include_expr(include_expr), main_expr(main_expr) {}
    
void global_expr_ast::accept(visitor& v) { v.visit(*this); }
    
unary_expr_ast::unary_expr_ast(const std::string& oper, ast* expr)
    : oper(oper), expr(expr) {}
    
void unary_expr_ast::accept(visitor& v) { v.visit(*this); }

typecast_ast::typecast_ast(type_ast* type_expr, ast* expr)
    : type_expr(type_expr), expr(expr) {}
    
void typecast_ast::accept(visitor& v) { v.visit(*this); }

num_ast::num_ast(const string& num_str)
    : num_str(num_str) {}
    
void num_ast::accept(visitor& v) { v.visit(*this); }

char_ast::char_ast(const string& char_str)
    : char_str(char_str) {}
    
void char_ast::accept(visitor& v) { v.visit(*this); }

string_ast::string_ast(const string& string_str)
    : string_str(string_str) {}
    
void string_ast::accept(visitor& v) { v.visit(*this); }

bool_ast::bool_ast(const bool val)
    : val(val) {}
    
void bool_ast::accept(visitor& v) { v.visit(*this); }

func_call_ast::func_call_ast(ast* func_expr, ast* paramlist)
    : func_expr(func_expr), paramlist(paramlist) {}
    
void func_call_ast::accept(visitor& v) { v.visit(*this); }

func_decl_ast::func_decl_ast(ident_ast* ident_expr, type_ast* ret_type, ast* arglist)
    : ident_expr(ident_expr), ret_type(ret_type), arglist(arglist) {}
    
void func_decl_ast::accept(visitor& v) { v.visit(*this); }

func_def_ast::func_def_ast(func_decl_ast* decl_expr, ast* impl_stmt)
    : decl_expr(decl_expr), impl_stmt(impl_stmt) {}
    
void func_def_ast::accept(visitor& v) { v.visit(*this); }

arg_ast::arg_ast(ident_ast* ident_expr, type_ast* type_expr)
    : ident_expr(ident_expr), type_expr(type_expr) {}
    
void arg_ast::accept(visitor& v) { v.visit(*this); }

type_ast::type_ast(const std::string& type_name, const yy::location& loc, int dimen, bool is_struct)
    : type_name(type_name), loc(loc), dimen(dimen), is_struct(is_struct) {}
    
void type_ast::accept(visitor& v) { v.visit(*this); }

struct_def_ast::struct_def_ast(ident_ast* ident_expr, ast* impl_expr)
    : ident_expr(ident_expr), impl_expr(impl_expr) {}
    
void struct_def_ast::accept(visitor& v) { v.visit(*this); }
    
struct_def_expr_ast::struct_def_expr_ast(const std::string& access, ast* def_expr)
    : access(access), def_expr(def_expr) {}
    
void struct_def_expr_ast::accept(visitor& v) { v.visit(*this); }

new_ast::new_ast(type_ast* type_expr, ast* arr_dimen_expr)
    : type_expr(type_expr), arr_dimen_expr(arr_dimen_expr) {}
    
void new_ast::accept(visitor& v) { v.visit(*this); }