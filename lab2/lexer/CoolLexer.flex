
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

\"                  {BEGIN(STRING);}  // Начало строки
<STRING>{
    \\b             {string_buf += "\b";}  // Обработка \b
    \\t             {string_buf += "\t";}  // Обработка \t
    \\n             {string_buf += "\n";}  // Обработка \n
    \\f             {string_buf += "\f";}  // Обработка \f
    \\\n            {/* Игнорируем экранированный перевод строки */}
    \\[^\n]         {string_buf += yytext[1];}  // Обработка \c (любой символ после \)
    \"              {BEGIN(INITIAL); return token::lit_string;}  // Конец строки
    \n              {Error("Unescaped newline in string");}  // Запрет на неэкранированный \n
    \0              {Error("Null character in string");}  // Запрет на \0
    <<EOF>>         {Error("EOF in string");}  // Запрет на EOF
    [^\\\n\"]+      {string_buf += yytext;}  // Обработка обычных символов
}

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

Object              {return token::bc_Object;}
Int                 {return token::bc_Int;}
String              {return token::bc_String;}
Bool                {return token::bc_Bool;}


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

\n                  {lineno++;} 
.                   {return token::UNKNOWN;}

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
