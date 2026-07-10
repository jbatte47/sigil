%{
#include "sigil.tab.h"
#include <string.h>

int indent_stack[100] = {0};
int indent_top = 0;
int dedents_to_emit = 0;

int current_line = 1;
int current_column = 1;

void push_indent(int indent) {
    indent_stack[++indent_top] = indent;
}

int pop_indent() {
    return indent_stack[indent_top--];
}

int top_indent() {
    return indent_stack[indent_top];
}

int paren_level = 0;

void update_pos() {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            current_line++;
            current_column = 1;
        } else {
            current_column++;
        }
    }
}

#define YY_USER_ACTION update_pos();
%}

%option noyywrap

%%

#[^\n]*            ; // ignore comments
^[ \t]*\n          ; // skip empty lines with spaces
\n[ \t]* {
    int indent = strlen(yytext) - 1;
    if (paren_level > 0) return NEWLINE; // Ignore indent changes inside groups
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
    return 0;
}

"manifests"         { return MANIFESTS; }
"incant"            { return INCANT; }
"if"                { return IF; }
"else"              { return ELSE; }
"as"                { return AS; }

"|"                 { return PIPE; }
"."                 { return DOT; }
":"                 { return COLON; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"("                 { paren_level++; return LPAREN; }
")"                 { paren_level--; return RPAREN; }
"["                 { paren_level++; return LBRACKET; }
"]"                 { paren_level--; return RBRACKET; }
"{"                 { paren_level++; return LBRACE; }
"}"                 { paren_level--; return RBRACE; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"%"                 { return PERCENT; }

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