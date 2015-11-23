/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* syntax analyser for leo (LEOpard) language */

%skeleton "lalr1.cc"
%require "3.0.4"
%defines
%define parser_class_name {leo_parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <cinttypes>
#include <cstdio>
#include <cstdlib>

class ast;
class func_decl_ast;
class type_ast;
class driver;
}

%param { driver& drv }

%locations
%initial-action {
@$.begin.filename = @$.end.filename = &drv.filename;
};

%define parse.trace
%define parse.error verbose

%code {
#include "../ast/ast.hh"
#include "../driver/driver.hh"
}

%define api.token.prefix {TOK_}
%token <std::string> INCLUDE
%token <std::string> IDENT
%token <std::string> STRUCT
%token <std::string> FUNC
%token <std::string> IF
%token <std::string> ELSE
%token <std::string> FOR
%token <std::string> DO 
%token <std::string> WHILE
%token <std::string> BREAK 
%token <std::string> CONTINUE
%token <std::string> RETURN
%token <std::string> BYTE
%token <std::string> INT8 
%token <std::string> INT16 
%token <std::string> INT32 
%token <std::string> INT64
%token <std::string> UINT8 
%token <std::string> UINT16 
%token <std::string> UINT32 
%token <std::string> UINT64
%token <std::string> CHAR
%token <std::string> BOOL
%token <std::string> LCHAR
%token <std::string> LSTRING
%token <std::string> NUMBER
%token TRUE FALSE LBRACE RBRACE LSQBRACE RSQBRACE LPAREN RPAREN COMMA COLON SEMICOLON
%token <std::string> NEW
%token <std::string> AT
%token <std::string> ADD
%token <std::string> SUB
%token <std::string> MUL
%token <std::string> DIV
%token <std::string> MOD
%token <std::string> BOR
%token <std::string> BAND
%token <std::string> BXOR
%token <std::string> BNOT
%token <std::string> LESS
%token <std::string> GTR
%token <std::string> ASSIGN
%token <std::string> NOT
%token <std::string> MEMBEROF
%token <std::string> ADDASSIGN
%token <std::string> SUBASSIGN
%token <std::string> MULASSIGN
%token <std::string> DIVASSIGN
%token <std::string> MODASSIGN
%token <std::string> BORASSIGN
%token <std::string> BANDASSIGN
%token <std::string> BXORASSIGN
%token <std::string> LSHIFT
%token <std::string> RSHIFT
%token <std::string> LSHIFTASSIGN
%token <std::string> RSHIFTASSIGN
%token <std::string> LESSEQ
%token <std::string> GTREQ
%token <std::string> EQ
%token <std::string> NEQ
%token <std::string> LOR
%token <std::string> LAND
%token EOF 0

%type <type_ast*> type
%type <func_decl_ast*> func_declaration
%type <ast*> leopard include_statement include_expression main_statement main_expression
%type <std::string> builtin_type access_expression binary_operator
%type <ast*> argument_statement argument_expression struct_definition struct_statement struct_expression
%type <ast*> func_expression func_statement common_expression common_statement
%type <ast*> func_definition ident_definition ifelse_expression
%type <ast*> for_expression for_init_expression foreach_expression dowhile_expression label_expression
%type <ast*> expression new_expression cast_expression unary_expression post_expression basic_expression return_expression
%type <ast*> continue_expression break_expression array_definition array_element_expression parameter_statement
%type <int> array_type

%nonassoc IFONLY
%nonassoc ELSE
%right ASSIGN ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN LSHIFTASSIGN RSHIFTASSIGN BANDASSIGN BXORASSIGN BORASSIGN
%left LOR
%left LAND
%left BOR
%left BXOR
%left BAND
%left EQ NEQ
%left LESS LESSEQ GTR GTREQ
%left LSHIFT RSHIFT
%left ADD SUB
%left MUL DIV MOD
%right NOT BNOT UPLUS UMINUS TYPECAST NEW
%left MEMBEROF SUBSCRIPT CALLFUNC

%printer { yyoutput << $$; } <std::string> <int>
%printer { $$ != nullptr? $$->ostream(yyoutput) : yyoutput << "<none>"; } <ast*> <type_ast*> <func_decl_ast*>

%%

%start leopard;

leopard: include_statement main_statement
         { $$ = new global_expr_ast($1, $2); drv.traverse($$); }
;

include_statement: /* empty */
                   { $$ = nullptr; }
                 | include_expression include_statement
                   { $$ = new stmt_list_ast($1, $2); }
;

include_expression: INCLUDE LSTRING SEMICOLON
                   { $$ = new unary_expr_ast($1, new string_ast($2)); }
;

main_statement: /* empty */
                { $$ = nullptr; }
              | main_expression main_statement
                { $$ = new stmt_list_ast($1, $2); }
;

main_expression: struct_definition
                 { $$ = std::move($1); }
               | func_definition
                 { $$ = std::move($1); }
;

array_type: /* empty */
              { $$ = 0; }
            | array_type "*"
              { $$ += 1; }
;

type: builtin_type array_type
      { $$ = new type_ast($1, @1, $2); }
    | STRUCT IDENT array_type
      { $$ = new type_ast($2, @2, $3, true); }
;

builtin_type: BYTE 
              { $$ = $1; }
            | INT8
              { $$ = $1; }
            | INT16
              { $$ = $1; }
            | INT32
              { $$ = $1; }
            | INT64
              { $$ = $1; }
            | UINT8
              { $$ = $1; }
            | UINT16
              { $$ = $1; }
            | UINT32
              { $$ = $1; }
            | UINT64
              { $$ = $1; }
            | CHAR
              { $$ = $1; }
            | BOOL
              { $$ = $1; }
;

argument_statement: /* empty */
                    { $$ = nullptr; }
                  | argument_expression
                    { $$ = std::move($1); }
                  | argument_statement COMMA argument_expression
                    { $$ = new stmt_list_ast($1, $3); }
;

argument_expression : IDENT COLON type
                      { $$ = new arg_ast(new ident_ast($1, @1), $3); }
;

struct_definition: STRUCT IDENT RPAREN struct_statement LPAREN
                   { $$ = new struct_def_ast(new ident_ast($2, @2), $4); }
;

struct_statement: /* empty */
                  { $$ = nullptr; }
                | struct_expression struct_statement
                  { $$ = new stmt_list_ast($1, $2); }
;

struct_expression: access_expression ident_definition SEMICOLON
                   { $$ = new struct_def_expr_ast($1, $2); }
                 | access_expression func_definition SEMICOLON
                   { $$ = new struct_def_expr_ast($1, $2); }
;

access_expression: /* empty */
                   { $$ = std::string("-"); }
                 | "-" /* private (default) */
                   { $$ = std::string("-"); }
                 | "+" /* public */
                   { $$ = std::string("+"); }
                 | "#" /* scope */
                   { $$ = std::string("#"); }
;

func_expression: return_expression SEMICOLON
                 { $$ = std::move($1); }
               | ident_definition
                 { $$ = std::move($1); }
               | expression SEMICOLON
                 { $$ = std::move($1); }
               | dowhile_expression SEMICOLON
                 { $$ = std::move($1); }
               | ifelse_expression
                 { $$ = std::move($1); }
               | for_expression
                 { $$ = std::move($1); }
               | foreach_expression
                 { $$ = std::move($1); }
               | label_expression
                 { $$ = std::move($1); }
;

func_statement: /* empty */
                { $$ = nullptr;}
              | func_expression func_statement
                { $$ = new stmt_list_ast($1, $2); }
;

common_expression: continue_expression SEMICOLON
                   { $$ = std::move($1); }
                 | break_expression SEMICOLON
                   { $$ = std::move($1); }
                 | return_expression SEMICOLON
                   { $$ = std::move($1); }
                 | ident_definition SEMICOLON
                   { $$ = std::move($1); }
                 | expression SEMICOLON
                   { $$ = std::move($1); }
                 | dowhile_expression SEMICOLON
                   { $$ = std::move($1); }
                 | ifelse_expression
                   { $$ = std::move($1); }
                 | for_expression
                   { $$ = std::move($1); }
                 | foreach_expression
                   { $$ = std::move($1); }
                 | label_expression
                   { $$ = std::move($1); }
;

common_statement: /* empty */
                  { $$ = nullptr; }
                | common_expression common_statement
                  { $$ = new stmt_list_ast($1, $2); }
;

func_declaration: FUNC IDENT COLON type LBRACE argument_statement RBRACE
                  { $$ = new func_decl_ast(new ident_ast($2, @2), $4, $6); }
                | FUNC IDENT LBRACE argument_statement RBRACE
                  { $$ = new func_decl_ast(new ident_ast($2, @2), nullptr, $4); }
;

func_definition: func_declaration LPAREN func_statement RPAREN
                 { $$ = new func_def_ast($1, $3); }
;

ident_definition: IDENT COLON type ASSIGN expression SEMICOLON
                  { $$ = new ident_def_ast(
                         new ident_decl_ast(new ident_ast($1, @1), $3),
                         $5); }
                | IDENT COLON type SEMICOLON
                  { $$ = new ident_decl_ast(new ident_ast($1, @1), $3); }
;

ifelse_expression: IF expression LPAREN common_statement RPAREN %prec IFONLY
                   { $$ = new branch_ast($2, $4); }
                 | IF expression LPAREN common_statement RPAREN ELSE ifelse_expression
                   { $$ = new branch_ast($2, $4, $7); }
                 | IF expression LPAREN common_statement RPAREN ELSE LPAREN common_statement RPAREN
                   { $$ = new branch_ast($2, $4, $8); }
;

for_expression: FOR for_init_expression SEMICOLON expression SEMICOLON expression LPAREN common_statement RPAREN
                { $$ = new loop_ast($2, $4, $8, $6); }
              | FOR expression LPAREN common_statement RPAREN
                { $$ = new loop_ast(nullptr, $2, $4, nullptr); }
;

for_init_expression: expression
                     { $$ = std::move($1); }
                   | ident_definition
                     { $$ = std::move($1); }
;

foreach_expression: FOR IDENT COLON IDENT LPAREN common_statement RPAREN
                    { $$ = new iter_loop_ast(new ident_ast($2, @2), new ident_ast($4, @4), $6); }
;

dowhile_expression: DO LPAREN common_statement RPAREN WHILE expression
                    { $$ = new loop_ast($3, $6, $3, nullptr); }
;

label_expression: AT IDENT COLON
                  { $$ = new unary_expr_ast($1, new ident_ast($2, @2)); }
;

binary_operator: ADD
                 { $$ = $1; }
               | SUB
                 { $$ = $1; }
               | MUL
                 { $$ = $1; }
               | DIV
                 { $$ = $1; }
               | MOD
                 { $$ = $1; }
               | BOR
                 { $$ = $1; }
               | BAND
                 { $$ = $1; }
               | BXOR
                 { $$ = $1; }
               | LESS
                 { $$ = $1; }
               | GTR
                 { $$ = $1; }
               | LSHIFT
                 { $$ = $1; }
               | RSHIFT
                 { $$ = $1; }
               | LESSEQ
                 { $$ = $1; }
               | GTREQ
                 { $$ = $1; }
               | EQ
                 { $$ = $1; }
               | NEQ
                 { $$ = $1; }
               | LOR
                 { $$ = $1; }
               | LAND
                 { $$ = $1; }
               | ASSIGN
                 { $$ = $1; }
               | ADDASSIGN
                 { $$ = $1; }
               | SUBASSIGN
                 { $$ = $1; }
               | MULASSIGN
                 { $$ = $1; }
               | DIVASSIGN
                 { $$ = $1; }
               | MODASSIGN
                 { $$ = $1; }
               | BORASSIGN
                 { $$ = $1; }
               | BANDASSIGN
                 { $$ = $1; }
               | BXORASSIGN
                 { $$ = $1; }
               | LSHIFTASSIGN
                 { $$ = $1; }
               | RSHIFTASSIGN
                 { $$ = $1; }
;

expression: unary_expression binary_operator expression 
            { $$ = new binary_expr_ast($2, $1, $3); }
          | cast_expression
            { $$ = std::move($1); } 
;

new_expression: NEW builtin_type array_definition
                { $$ = new new_ast(new type_ast($2, @2), $3); }
              | NEW STRUCT IDENT array_definition
                { $$ = new new_ast(new type_ast($3, @3, 0, true), $4); }
;

cast_expression: LBRACE type RBRACE cast_expression %prec TYPECAST
                 { $$ = new typecast_ast($2, $4); }
               | unary_expression
                 { $$ = std::move($1); }

unary_expression: LBRACE "+" cast_expression RBRACE %prec UPLUS
                  { $$ = new unary_expr_ast("+", $3); }
                | LBRACE "-" cast_expression RBRACE %prec UMINUS
                  { $$ = new unary_expr_ast("-", $3); }
                | BNOT cast_expression
                  { $$ = new unary_expr_ast($1, $2); }
                | NOT cast_expression
                  { $$ = new unary_expr_ast($1, $2); }
                | post_expression
                  { $$ = std::move($1); }
                | new_expression
                  { $$ = std::move($1); }
;

post_expression: basic_expression
                 { $$ = std::move($1); }
               | post_expression LBRACE parameter_statement RBRACE %prec CALLFUNC
                 { $$ = new func_call_ast($1, $3); }
               | post_expression LSQBRACE expression RSQBRACE %prec SUBSCRIPT
                 { $$ = new binary_expr_ast("[]", $1, $3); }
               | post_expression MEMBEROF IDENT
                 { $$ = new binary_expr_ast($2, $1, new ident_ast($3, @3)); }
               | LPAREN array_element_expression RPAREN
                 { $$ = std::move($2); }
;

basic_expression: IDENT
                  { $$ = new ident_ast($1, @1); }
                | NUMBER
                  { $$ = new num_ast($1); }
                | LCHAR
                  { $$ = new char_ast($1); }
                | LSTRING
                  { $$ = new string_ast($1); }
                | TRUE
                  { $$ = new bool_ast(true); }
                | FALSE
                  { $$ = new bool_ast(false); }
                | LBRACE expression RBRACE
                  { $$ = std::move($2); }
;

return_expression: RETURN
                   { $$ = new unary_expr_ast($1, nullptr); }
                 | RETURN expression
                   { $$ = new unary_expr_ast($1, $2); }
;

continue_expression: CONTINUE
                     { $$ = new unary_expr_ast($1, nullptr); }
                   | CONTINUE IDENT
                     { $$ = new unary_expr_ast($1, new ident_ast($2, @2)); }
;

break_expression: BREAK
                  { $$ = new unary_expr_ast($1, nullptr); }
                | BREAK IDENT
                  { $$ = new unary_expr_ast($1, new ident_ast($2, @2)); }
;

array_definition: /* empty */
                  { $$ = nullptr; }
                | array_definition LSQBRACE expression RSQBRACE
                  { $$ = new stmt_list_ast($1, $3); }
;

array_element_expression: expression
                          { $$ = std::move($1); }
                        | array_element_expression COMMA expression
                          { $$ = new stmt_list_ast($1, $3); }
;

parameter_statement: /* empty */
                     { $$ = nullptr; }
                   | expression
                     { $$ = std::move($1); }
                   | parameter_statement COMMA expression
                     { $$ = new stmt_list_ast($1, $3); }
;

%%

void yy::leo_parser::error(const location_type& lt, const std::string& info) {
    
    drv.error(lt, info);
}