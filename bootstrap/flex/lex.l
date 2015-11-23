/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* lexical specification for leo (LEOpard) language */
%{
#include <cerrno>
#include "../driver/driver.hh"
#include "../logger/logger.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
#undef yywrap
#define yywrap() 1

static yy::location loc;
static int prev_state;
%}

%option noyywrap nounput batch debug noinput

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%{
//Code run each time a pattern is matched.
#define YY_USER_ACTION  loc.columns(yyleng);
%}

%x COMMENT

%%

%{
//Code run each time yylex is called.
loc.step();
%}

 /* operators */
"@"            { return yy::leo_parser::make_AT(yytext, loc); }
"("            { return yy::leo_parser::make_LBRACE(loc); }
")"            { return yy::leo_parser::make_RBRACE(loc); }
"["            { return yy::leo_parser::make_LSQBRACE(loc); }
"]"            { return yy::leo_parser::make_RSQBRACE(loc); }
"}"            { return yy::leo_parser::make_RPAREN(loc); }
"{"            { return yy::leo_parser::make_LPAREN(loc); }
","            { return yy::leo_parser::make_COMMA(loc); }
":"            { return yy::leo_parser::make_COLON(loc); }
";"            { return yy::leo_parser::make_SEMICOLON(loc); }
"+"            { return yy::leo_parser::make_ADD(yytext, loc); }
"-"            { return yy::leo_parser::make_SUB(yytext, loc); }
"*"            { return yy::leo_parser::make_MUL(yytext, loc); }
"/"            { return yy::leo_parser::make_DIV(yytext, loc); }
"%"            { return yy::leo_parser::make_MOD(yytext, loc); }
"|"            { return yy::leo_parser::make_BOR(yytext, loc); }
"&"            { return yy::leo_parser::make_BAND(yytext, loc); }
"^"            { return yy::leo_parser::make_BXOR(yytext, loc); }
"~"            { return yy::leo_parser::make_BNOT(yytext, loc); }
"<"            { return yy::leo_parser::make_LESS(yytext, loc); }
">"            { return yy::leo_parser::make_GTR(yytext, loc); }
"="            { return yy::leo_parser::make_ASSIGN(yytext, loc); }
"!"            { return yy::leo_parser::make_NOT(yytext, loc); }
"."            { return yy::leo_parser::make_MEMBEROF(yytext, loc); }
"+="           { return yy::leo_parser::make_ADDASSIGN(yytext, loc); }
"-="           { return yy::leo_parser::make_SUBASSIGN(yytext, loc); }
"*="           { return yy::leo_parser::make_MULASSIGN(yytext, loc); }
"/="           { return yy::leo_parser::make_DIVASSIGN(yytext, loc); }
"%="           { return yy::leo_parser::make_MODASSIGN(yytext, loc); }
"|="           { return yy::leo_parser::make_BORASSIGN(yytext, loc); }
"&="           { return yy::leo_parser::make_BANDASSIGN(yytext, loc); }
"^="           { return yy::leo_parser::make_BXORASSIGN(yytext, loc); }
"<<"           { return yy::leo_parser::make_LSHIFT(yytext, loc); }
">>"           { return yy::leo_parser::make_RSHIFT(yytext, loc); }
"<<="          { return yy::leo_parser::make_LSHIFTASSIGN(yytext, loc); }
">>="          { return yy::leo_parser::make_RSHIFTASSIGN(yytext, loc); }
"<="           { return yy::leo_parser::make_LESSEQ(yytext, loc); }
">="           { return yy::leo_parser::make_GTREQ(yytext, loc); }
"=="           { return yy::leo_parser::make_EQ(yytext, loc); }
"!="           { return yy::leo_parser::make_NEQ(yytext, loc); }
"||"           { return yy::leo_parser::make_LOR(yytext, loc); }
"&&"           { return yy::leo_parser::make_LAND(yytext, loc); }

 /* keywords */
"include"      { return yy::leo_parser::make_INCLUDE(yytext, loc); }
"struct"       { return yy::leo_parser::make_STRUCT(yytext, loc); }
"func"         { return yy::leo_parser::make_FUNC(yytext, loc); }
"if"           { return yy::leo_parser::make_IF(yytext, loc); }
"else"         { return yy::leo_parser::make_ELSE(yytext, loc); }
"for"          { return yy::leo_parser::make_FOR(yytext, loc); }
"do"           { return yy::leo_parser::make_DO(yytext, loc);}
"while"        { return yy::leo_parser::make_WHILE(yytext, loc); }
"break"        { return yy::leo_parser::make_BREAK(yytext, loc); }
"continue"     { return yy::leo_parser::make_CONTINUE(yytext, loc); }
"return"       { return yy::leo_parser::make_RETURN(yytext, loc); }
"byte"         { return yy::leo_parser::make_BYTE(yytext, loc); }
"int8"         { return yy::leo_parser::make_INT8(yytext, loc); }
"int16"        { return yy::leo_parser::make_INT16(yytext, loc); }
"int32"        { return yy::leo_parser::make_INT32(yytext, loc); }
"int64"        { return yy::leo_parser::make_INT64(yytext, loc); }
"uint8"        { return yy::leo_parser::make_UINT8(yytext, loc); }
"uint16"       { return yy::leo_parser::make_UINT16(yytext, loc); }
"uint32"       { return yy::leo_parser::make_UINT32(yytext, loc); }
"uint64"       { return yy::leo_parser::make_UINT64(yytext, loc); }
"char"         { return yy::leo_parser::make_CHAR(yytext, loc); }
"bool"         { return yy::leo_parser::make_BOOL(yytext, loc); }
"true"         { return yy::leo_parser::make_TRUE(loc); }
"false"        { return yy::leo_parser::make_FALSE(loc); }
"new"          { return yy::leo_parser::make_NEW(yytext, loc); }

 /* identifier */
"_"[a-zA-Z0-9_]+            |
[a-zA-Z][a-zA-Z0-9_]*       { return yy::leo_parser::make_IDENT(yytext, loc); }

 /* literal number */
("0"|[1-9]+)"."[0-9]+{EXP}? |
"0"|([1-9][0-9]*{EXP}?)     |
"0"[1-7][0-7]*              |
"0"[xX][0-9A-Fa-f]+         { return yy::leo_parser::make_NUMBER(yytext, loc); }

 /* literal character */
"'"."'"                     |
"\""."\""                   { return yy::leo_parser::make_LCHAR(yytext, loc); }

 /* literal string */
"`".*"`"                    { return yy::leo_parser::make_LSTRING(yytext, loc); }

 /* single line comment */
"//".*                      { loc.step(); }

 /* multi-line comment */
"/*"             { prev_state = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"    { BEGIN prev_state; }
<COMMENT>.       { loc.step(); }
<COMMENT>\n      { loc.lines(yyleng); loc.step(); }
<COMMENT><<EOF>> { drv.error(loc, "unclosed comment"); }

[ \t\r]+         { loc.step(); }
[\n]+            { loc.lines(yyleng); loc.step(); }
.                { drv.error(loc, "invalid character:" + std::string(yytext)); }
<<EOF>>          { return yy::leo_parser::make_EOF(loc); }

%%

void driver::begin_scan() {
    
    LOGGER_INFO("Start scanning");
    LOGGER_DEBUG("Trace tokens:" + std::string(trace_scanning? "true" : "false"));
    yy_flex_debug = trace_scanning;
    if(filename.empty() || filename == "-") {
        LOGGER_INFO("no file specified scan from console");
        yyin = stdin;
    } else if(!(yyin = fopen(filename.c_str(), "r"))) {
        error("cannot open" + filename + ":" + strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void driver::end_scan() {

    LOGGER_INFO("End scanning");
    fclose(yyin);
}