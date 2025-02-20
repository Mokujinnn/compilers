
%option noyywrap c++
%option yylineno
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

white_space       [ \t]*
digit0            [0-9]
digit1            [1-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit0})
identifier        {alpha}{alpha_num}*


%%

{white_space}       /*skip*/

"class"             {return TOKEN_KW_class;}
{identifier}        {return TOKEN_identifier;}
"{"                 {return TOKEN_LEFT_BRACES;}
"}"                 {return TOKEN_RIGHT_BRACES;}

\n                  {lineno++;} 
.                   {return TOKEN_UNKNOWN;}

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}
