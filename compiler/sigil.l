%{
#include "sigil.tab.h"  // Bison-generated token header
#include <string.h>

int indent_level = 0;
%}

%option noyywrap

%%

[ \t]+              ;  // ignore regular whitespace
\n                  { return NEWLINE; }

"incant"            { return INCANT; }
"given"             { return GIVEN; }
"invoke"            { return INVOKE; }
"essence:"          { return ESSENCE; }
"as"                { return AS; }
"bind"              { return BIND; }
"to"                { return TO; }
"yield"             { return YIELD; }
"of"                { return OF; }
":"                 { return COLON; }
";"                 { return SEMICOLON; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

.                   ;  // catch all / ignore unknowns for now

%%