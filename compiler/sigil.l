%{
#include "sigil.tab.h"
#include <string.h>

int indent_stack[100] = {0};
int indent_top = 0;
int dedents_to_emit = 0;

void push_indent(int indent) {
    indent_stack[++indent_top] = indent;
}

int pop_indent() {
    return indent_stack[indent_top--];
}

int top_indent() {
    return indent_stack[indent_top];
}
%}

%option noyywrap

%%

#[^\n]*            ; // ignore comments
^[ \t]*\n          ; // skip empty lines with spaces
\n[ \t]* {
    int indent = strlen(yytext) - 1;
    if (indent > top_indent()) {
        push_indent(indent);
        return INDENT;
    } else if (indent < top_indent()) {
        while (indent < top_indent()) {
            pop_indent();
            dedents_to_emit++;
        }
        if (dedents_to_emit > 0) {
            dedents_to_emit--;
            unput('\n'); 
            for (int i=0; i<indent; i++) unput(' ');
            return DEDENT;
        }
    }
    return NEWLINE;
}

<<EOF>> {
    if (top_indent() > 0) {
        pop_indent();
        return DEDENT;
    }
    yyterminate();
}

"incant"            { return INCANT; }
"manifest"          { return MANIFEST; }
"imbue"             { return IMBUE; }
"into"              { return INTO; }
"on"                { return ON; }
"given"             { return GIVEN; }
"yield"             { return YIELD; }
"as"                { return AS; }
"invoke"            { return INVOKE; }
"import"            { return IMPORT; }
"export"            { return EXPORT; }
"instance"          { return INSTANCE; }
"from"              { return FROM; }
"let"               { return LET; }
"with"              { return WITH; }
"gesture"           { return GESTURE; }
"->"                { return ARROW; }
"."                 { return DOT; }
":"                 { return COLON; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"("                 { return LPAREN; }
")"                 { return RPAREN; }
"="                 { return EQUALS; }
"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }

[0-9]+(\.[0-9]+)?   { 
    yylval.str = strdup(yytext); 
    return NUMBER; 
}

\"[^\"]*\"|'[^']*' {
    yylval.str = strndup(yytext + 1, strlen(yytext) - 2);
    return STRING;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}

[ \t]+              ; // ignore spaces in mid-line

.                   ;

%%