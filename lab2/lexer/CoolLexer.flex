
%option noyywrap c++
%option yyclass="CoolLexer"


%{

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()

%}

white_space       [ \t\f\r\v]*
digit0            [0-9]
digit1            [1-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit0})
identifier        {alpha}{alpha_num}*
lit_int           ({digit0}|{digit1}{digit0}*)

%x COMMENT
%x STRING

%%

"--".*              /*skip*/

"(*"                {BEGIN(COMMENT); MultiLineCommentStart();}
<COMMENT>{
    "(*"            {MultiLineCommentStart();}
    "*)"            {if (MultiLineCommentEnd()) BEGIN(INITIAL);}
    \n              {++lineno;}
    <<EOF>>         {Error("EOF in multi line comment");}
    .               /*skip*/
}
"*)"                {Error("Comment end without start");}

\"                  {BEGIN(STRING);}
<STRING>{
    \\b             {string_buf += "\b";}
    \\t             {string_buf += "\t";}
    \\n             {string_buf += "\n"; ++lineno;}
    \\f             {string_buf += "\f";}
    \\\n            {}
    \\[^\n]         {string_buf += yytext[1];}
    \"              {BEGIN(INITIAL); return token::lit_string;}
    \n              {Error("Unescaped newline in string");}
    \0              {Error("Null character in string");}
    <<EOF>>         {Error("EOF in string");}
    [^\\\n\"]+      {string_buf += yytext;}
}

{lit_int}           {return token::lit_int;}

{white_space}       /*skip*/

class               {return token::kw_class;}
else                {return token::kw_else;}
false               {return token::kw_false;}
fi                  {return token::kw_fi;}
if                  {return token::kw_if;}
in                  {return token::kw_in;}
inherits            {return token::kw_inherits;}
isvoid              {return token::kw_isvoid;}
let                 {return token::kw_let;}
loop                {return token::kw_loop;}
pool                {return token::kw_pool;}
then                {return token::kw_then;}
while               {return token::kw_while;}
case                {return token::kw_case;}
esac                {return token::kw_esac;}
new                 {return token::kw_new;}
of                  {return token::kw_of;}
not                 {return token::kw_not;}
true                {return token::kw_true;}

Object              {return token::bo_Object;}
Int                 {return token::bo_Int;}
String              {return token::bo_String;}
Bool                {return token::bo_Bool;}

SELF_TYPE           {return token::self_type;}
self                {return token::self;}

{identifier}        {return token::identifier;}


"<-"                {return token::expr_assignment;}

"+"                 {return token::binop_plus;}
"-"                 {return token::binop_minus;}
"*"                 {return token::binop_mul;}
"/"                 {return token::binop_div;}
"<"                 {return token::binop_l;}
"<="                {return token::binop_le;}
"="                 {return token::binop_eq;}


"{"                 {return token::l_brace;}
"}"                 {return token::r_brace;}
"("                 {return token::l_paren;}
")"                 {return token::r_paren;}

":"                 {return token::colon;}
";"                 {return token::semicolon;}
"."                 {return token::dot;}
","                 {return token::comma;}

\n                  {lineno++;} 
.                   {return token::UNKNOWN;}

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
