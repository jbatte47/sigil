%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode* root;
%}

%union {
    char* str;
    struct ASTNode* node;
}

%token <str> IDENTIFIER NUMBER STRING
%token INCANT MANIFEST IMBUE INTO ON GIVEN YIELD AS INVOKE 
%token IMPORT EXPORT INSTANCE FROM LET WITH GESTURE
%token ARROW DOT COLON SEMICOLON COMMA EQUALS PLUS MINUS STAR SLASH
%token NEWLINE INDENT DEDENT LPAREN RPAREN

%type <node> program declaration_list declaration import_decl export_decl instance_decl
%type <node> instance_init instance_decorators decorator
%type <node> incant_decl opt_params manifest_decl imbue_decl imbue_content 
%type <node> statement_block statement_list statement expression
%type <node> binary_expr term factor
%type <node> primary_expr call_args call_arg_list call_arg opt_call_args
%type <node> param_list param identifier_list simulation_block gesture_stmt invoke_stat

%left PLUS MINUS
%left STAR SLASH
%right ARROW
%left DOT LPAREN

%%

program:
    declaration_list { root = $1; }
    ;

declaration_list:
    declaration { $$ = $1; }
    | declaration_list declaration {
        if ($2) {
            if ($1) add_next($1, $2);
            else $1 = $2;
        }
        $$ = $1;
    }
    | declaration_list NEWLINE { $$ = $1; }
    ;

declaration:
    import_decl { $$ = $1; }
    | export_decl { $$ = $1; }
    | instance_decl { $$ = $1; }
    | incant_decl { $$ = $1; }
    | manifest_decl { $$ = $1; }
    | imbue_decl { $$ = $1; }
    | invoke_stat { $$ = $1; }
    | simulation_block { $$ = $1; }
    | NEWLINE { $$ = NULL; }
    ;

import_decl:
    IMPORT identifier_list FROM IDENTIFIER SEMICOLON {
        $$ = create_node(NODE_IMPORT, $4, $2, NULL);
    }
    ;

export_decl:
    EXPORT declaration {
        $$ = create_node(NODE_EXPORT, NULL, $2, NULL);
    }
    ;

instance_decl:
    INSTANCE IDENTIFIER COLON IDENTIFIER instance_init instance_decorators SEMICOLON {
        ASTNode* id = create_node(NODE_IDENTIFIER, $4, NULL, NULL);
        if ($5) add_next(id, $5);
        if ($6) add_next(id, $6);
        $$ = create_node(NODE_INSTANCE, $2, id, NULL);
    }
    | INSTANCE IDENTIFIER COLON IDENTIFIER instance_init instance_decorators {
        ASTNode* id = create_node(NODE_IDENTIFIER, $4, NULL, NULL);
        if ($5) add_next(id, $5);
        if ($6) add_next(id, $6);
        $$ = create_node(NODE_INSTANCE, $2, id, NULL);
    }
    ;

instance_init:
    LPAREN call_arg_list RPAREN { $$ = $2; }
    | /* empty */ { $$ = NULL; }
    ;

instance_decorators:
    instance_decorators decorator {
        if ($1) { add_next($1, $2); $$ = $1; }
        else $$ = $2;
    }
    | /* empty */ { $$ = NULL; }
    ;

decorator:
    DOT IDENTIFIER LPAREN call_arg_list RPAREN {
        $$ = create_node(NODE_CALL, $2, $4, NULL);
    }
    ;

incant_decl:
    INCANT IDENTIFIER opt_params COLON statement_block {
        $$ = create_node(NODE_INCANTATION, $2, $3, $5);
    }
    ;

opt_params:
    LPAREN param_list RPAREN { $$ = $2; }
    | LPAREN RPAREN { $$ = NULL; }
    | /* empty */ { $$ = NULL; }
    ;

manifest_decl:
    MANIFEST IDENTIFIER AS IDENTIFIER SEMICOLON {
        $$ = create_node(NODE_MANIFEST, $2, create_node(NODE_IDENTIFIER, $4, NULL, NULL), NULL);
    }
    ;

imbue_decl:
    IMBUE INTO IDENTIFIER COLON imbue_content {
        $$ = create_node(NODE_IMBUE, $3, NULL, $5);
    }
    ;

imbue_content:
    INDENT statement_list DEDENT { $$ = $2; }
    | NEWLINE INDENT statement_list DEDENT { $$ = $3; }
    ;

statement_block:
    INDENT statement_list DEDENT { $$ = $2; }
    | NEWLINE INDENT statement_list DEDENT { $$ = $3; }
    | statement_list { $$ = $1; }
    ;

statement_list:
    statement { $$ = $1; }
    | statement_list statement {
        if ($2) {
            if ($1) add_next($1, $2);
            else $1 = $2;
        }
        $$ = $1;
    }
    ;

statement:
    LET IDENTIFIER EQUALS expression { $$ = create_node(NODE_LET, $2, $4, NULL); }
    | expression { $$ = $1; }
    | YIELD expression { $$ = create_node(NODE_YIELD, NULL, $2, NULL); }
    | gesture_stmt { $$ = $1; }
    | NEWLINE { $$ = NULL; }
    | SEMICOLON { $$ = NULL; }
    ;

expression:
    binary_expr { $$ = $1; }
    | expression ARROW binary_expr { $$ = create_node(NODE_FLOW, NULL, $1, $3); }
    ;

binary_expr:
    term { $$ = $1; }
    | binary_expr PLUS term { $$ = create_node(NODE_BINOP, "+", $1, $3); }
    | binary_expr MINUS term { $$ = create_node(NODE_BINOP, "-", $1, $3); }
    ;

term:
    factor { $$ = $1; }
    | term STAR factor { $$ = create_node(NODE_BINOP, "*", $1, $3); }
    | term SLASH factor { $$ = create_node(NODE_BINOP, "/", $1, $3); }
    ;

factor:
    primary_expr { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

primary_expr:
    IDENTIFIER { $$ = create_node(NODE_IDENTIFIER, $1, NULL, NULL); }
    | NUMBER { $$ = create_node(NODE_LITERAL, $1, NULL, NULL); }
    | STRING { $$ = create_node(NODE_LITERAL, $1, NULL, NULL); }
    | primary_expr DOT IDENTIFIER { 
        $$ = create_node(NODE_MEMBER_ACCESS, $3, $1, NULL); 
    }
    | primary_expr DOT GESTURE { 
        $$ = create_node(NODE_MEMBER_ACCESS, "gesture", $1, NULL); 
    }
    | primary_expr call_args { 
        $$ = create_node(NODE_CALL, NULL, $1, $2); 
    }
    ;

opt_call_args:
    LPAREN call_arg_list RPAREN { $$ = $2; }
    | /* empty */ { $$ = NULL; }
    ;

call_args:
    LPAREN call_arg_list RPAREN { $$ = $2; }
    ;

call_arg_list:
    call_arg { $$ = $1; }
    | call_arg_list COMMA call_arg {
        add_next($1, $3);
        $$ = $1;
    }
    | call_arg_list PLUS call_arg {
        $$ = create_node(NODE_BINOP, "+", $1, $3);
    }
    | /* empty */ { $$ = NULL; }
    ;

call_arg:
    IDENTIFIER EQUALS expression { $$ = create_node(NODE_NAMED_ARG, $1, $3, NULL); }
    | expression { $$ = $1; }
    ;

simulation_block:
    GIVEN expression COLON statement_block {
        $$ = create_node(NODE_GIVEN_BLOCK, NULL, $2, $4);
    }
    ;

gesture_stmt:
    GESTURE opt_call_args WITH IDENTIFIER opt_call_args SEMICOLON {
        ASTNode* args = $2;
        if ($5) {
            if (args) add_next(args, $5);
            else args = $5;
        }
        $$ = create_node(NODE_GESTURE_STMT, $4, args, NULL);
    }
    | GESTURE opt_call_args WITH IDENTIFIER opt_call_args {
        ASTNode* args = $2;
        if ($5) {
            if (args) add_next(args, $5);
            else args = $5;
        }
        $$ = create_node(NODE_GESTURE_STMT, $4, args, NULL);
    }
    ;

invoke_stat:
    INVOKE IDENTIFIER SEMICOLON {
        $$ = create_node(NODE_INVOKE, $2, NULL, NULL);
    }
    ;

param_list:
    param { $$ = $1; }
    | param_list COMMA param {
        add_next($1, $3);
        $$ = $1;
    }
    ;

param:
    IDENTIFIER COLON IDENTIFIER {
        $$ = create_node(NODE_PARAM, $1, create_node(NODE_IDENTIFIER, $3, NULL, NULL), NULL);
    }
    ;

identifier_list:
    IDENTIFIER { $$ = create_node(NODE_IDENTIFIER, $1, NULL, NULL); }
    | identifier_list COMMA IDENTIFIER {
        add_next($1, create_node(NODE_IDENTIFIER, $3, NULL, NULL));
        $$ = $1;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
