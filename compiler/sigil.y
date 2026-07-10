%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int current_line;
extern int current_column;
void yyerror(const char *s);

ASTNode* root;

#define MKNODE(t, v, l, r) create_node_loc(t, v, l, r, current_line, current_column)
%}

%union {
    char* str;
    struct ASTNode* node;
}

%token <str> IDENTIFIER NUMBER STRING
%token MANIFESTS INCANT
%token <str> TYPE INSTANCE
%token IF ELSE AS
%token PIPE DOT COLON SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
%token PLUS MINUS STAR SLASH PERCENT
%token CARET CARET_TILDE
%token NEWLINE INDENT DEDENT


%type <node> manifest_decl module_ref manifest_symbol_list
%type <node> incant_decl param_list_opt param_list param
%type <node> type_decl instance_decl metadata_prefix

%type <node> block_like pipeline_seq pipeline_item
%type <node> expression pipe_expr ternary_expr postfix_expr atom_expr
%type <node> object_literal field_list_opt field_list field_item field
%type <node> stmt_end_opt
%type <str> name_token

%type <node> paren_group prefix_call callable_expr lambda_literal lambda_body pipe_op
%type <node> list_literal list_items_opt list_items list_item
%type <node> arg_list_opt arg_list

%%

program:
    /* empty */
    | program manifest_decl { if ($2) { if (root) add_next(root, $2); else root = $2; } }
    | program incant_decl   { if ($2) { if (root) add_next(root, $2); else root = $2; } }
    | program type_decl     { if ($2) { if (root) add_next(root, $2); else root = $2; } }
    | program instance_decl { if ($2) { if (root) add_next(root, $2); else root = $2; } }
    | program expression    { if ($2) { if (root) add_next(root, $2); else root = $2; } }
    | program NEWLINE
    | program SEMICOLON
    ;

manifest_decl:
    module_ref MANIFESTS LBRACKET manifest_symbol_list RBRACKET stmt_end_opt {
        $$ = MKNODE(NODE_MANIFEST_DECL, NULL, $1, $4);
    }
    ;

module_ref:
    name_token { $$ = MKNODE(NODE_MODULE_REF, $1, NULL, NULL); }
    | name_token LPAREN STRING RPAREN {
        $$ = MKNODE(NODE_MODULE_REF, $1, MKNODE(NODE_LITERAL, $3, NULL, NULL), NULL); 
    }
    ;

manifest_symbol_list:
    name_token { $$ = MKNODE(NODE_IDENTIFIER, $1, NULL, NULL); }
    | manifest_symbol_list COMMA name_token {
        add_next($1, MKNODE(NODE_IDENTIFIER, $3, NULL, NULL));
        $$ = $1;
    }
    ;

incant_decl:
    INCANT IDENTIFIER LBRACKET param_list_opt RBRACKET block_like {
        $$ = MKNODE(NODE_INCANT_DECL, $2, $4, $6);
    }
    ;

type_decl:
    TYPE name_token stmt_end_opt {
        $$ = MKNODE(NODE_TYPE_DECL, $2, NULL, NULL);
    }
    | TYPE name_token metadata_prefix stmt_end_opt {
        $$ = MKNODE(NODE_TYPE_DECL, $2, $3, NULL);
    }
    ;

instance_decl:
    INSTANCE name_token COLON name_token stmt_end_opt {
        $$ = MKNODE(NODE_INSTANCE_DECL, $2, MKNODE(NODE_IDENTIFIER, $4, NULL, NULL), NULL);
    }
    | INSTANCE metadata_prefix name_token COLON name_token stmt_end_opt {
        $$ = MKNODE(NODE_INSTANCE_DECL, $3, MKNODE(NODE_IDENTIFIER, $5, NULL, NULL), $2);
    }
    ;

metadata_prefix:
    CARET object_literal {
        $$ = $2;
    }
    | CARET_TILDE object_literal {
        $$ = $2;
        if ($$) {
            $$->value = strdup("override");
        }
    }
    ;

param_list_opt:
    /* empty */ { $$ = NULL; }
    | param_list { $$ = $1; }
    ;

param_list:
    param { $$ = $1; }
    | param_list COMMA param {
        add_next($1, $3);
        $$ = $1;
    }
    ;

param:
    name_token COLON name_token {
        $$ = MKNODE(NODE_PARAM, $1, MKNODE(NODE_IDENTIFIER, $3, NULL, NULL), NULL);
    }
    ;



block_like:
    expression { $$ = $1; }
    | INDENT expression DEDENT { $$ = $2; }
    | INDENT pipeline_seq DEDENT { $$ = MKNODE(NODE_BLOCK, NULL, $2, NULL); }
    ;

pipeline_seq:
    pipeline_item { $$ = $1; }
    | pipeline_seq pipeline_sep pipeline_item {
        if ($3) {
            if ($1) {
                // If it was a pipe or we want to force pipeline behavior
                $$ = MKNODE(NODE_PIPELINE, NULL, $1, $3);
            } else $$ = $3;
        }
        $$ = $1;
    }
    ;

pipeline_sep:
    NEWLINE
    | PIPE
    | NEWLINE PIPE
    ;

pipeline_item:
    expression { $$ = $1; }
    ;

expression:
    pipe_expr { $$ = $1; }
    ;

pipe_expr:
    ternary_expr { $$ = $1; }
    | pipe_expr pipe_op ternary_expr {
        $$ = MKNODE(NODE_PIPELINE, NULL, $1, $3);
    }
    ;

pipe_op:
    PIPE { $$ = NULL; }
    | NEWLINE PIPE { $$ = NULL; }
    | PIPE NEWLINE { $$ = NULL; }
    | NEWLINE PIPE NEWLINE { $$ = NULL; }
    ;

ternary_expr:
    postfix_expr { $$ = $1; }
    | postfix_expr IF expression ELSE expression {
        ASTNode* cond_block = $3;
        add_next(cond_block, $5);
        $$ = MKNODE(NODE_TERNARY, NULL, $1, cond_block);
    }
    ;

postfix_expr:
    atom_expr { $$ = $1; }
    | postfix_expr DOT name_token { $$ = MKNODE(NODE_MEMBER_ACCESS, $3, $1, NULL); }
    | postfix_expr object_literal { $$ = MKNODE(NODE_RECORD_CALL, NULL, $1, $2); }
    ;

arg_list_opt:
    /* empty */ { $$ = NULL; }
    | arg_list { $$ = $1; }
    ;

arg_list:
    expression { $$ = $1; }
    | arg_list expression { add_next($1, $2); $$ = $1; }
    | arg_list COMMA expression { add_next($1, $3); $$ = $1; }
    ;

atom_expr:
    name_token { $$ = MKNODE(NODE_IDENTIFIER, $1, NULL, NULL); }
    | NUMBER { $$ = MKNODE(NODE_LITERAL, $1, NULL, NULL); }
    | STRING { $$ = MKNODE(NODE_LITERAL, $1, NULL, NULL); }
    | COLON name_token { $$ = MKNODE(NODE_IDENTIFIER, $2, NULL, NULL); }
    | paren_group { $$ = $1; }
    | prefix_call { $$ = $1; }
    | lambda_literal { $$ = $1; }
    | list_literal { $$ = $1; }
    | object_literal { $$ = $1; }
    ;

paren_group:
    LPAREN expression RPAREN { $$ = $2; }
    ;

prefix_call:
    LPAREN callable_expr arg_list_opt RPAREN {
        $$ = MKNODE(NODE_PREFIX_CALL, NULL, $2, $3);
    }
    ;

callable_expr:
    postfix_expr { $$ = $1; }
    | PLUS { $$ = MKNODE(NODE_IDENTIFIER, "+", NULL, NULL); }
    | MINUS { $$ = MKNODE(NODE_IDENTIFIER, "-", NULL, NULL); }
    | STAR { $$ = MKNODE(NODE_IDENTIFIER, "*", NULL, NULL); }
    | SLASH { $$ = MKNODE(NODE_IDENTIFIER, "/", NULL, NULL); }
    | PERCENT { $$ = MKNODE(NODE_IDENTIFIER, "%", NULL, NULL); }
    ;

lambda_literal:
    LPAREN LBRACKET lambda_body RBRACKET RPAREN { $$ = $3; }
    | LBRACKET LBRACKET lambda_body RBRACKET RBRACKET { $$ = $3; }
    ;

lambda_body:
    param_list COMMA expression { $$ = MKNODE(NODE_LAMBDA, NULL, $1, $3); }
    | LBRACKET param_list RBRACKET COMMA expression { $$ = MKNODE(NODE_LAMBDA, NULL, $2, $5); }
    ;

list_literal:
    LBRACKET list_sep_opt list_items_opt list_sep_opt RBRACKET { $$ = MKNODE(NODE_LIST_LITERAL, NULL, $3, NULL); }
    ;

list_sep_opt:
    /* empty */
    | COMMA
    | NEWLINE
    | COMMA NEWLINE
    ;

list_items_opt:
    /* empty */ { $$ = NULL; }
    | list_items { $$ = $1; }
    ;

list_items:
    list_item { $$ = $1; }
    | list_items COMMA list_item { add_next($1, $3); $$ = $1; }
    | list_items list_item { add_next($1, $2); $$ = $1; }
    ;

list_item:
    name_token AS expression {
        $$ = MKNODE(NODE_LIST_ITEM_ALIAS, $1, $3, NULL); 
    }
    | expression { $$ = $1; }
    ;

object_literal:
    LBRACE field_list_opt RBRACE { $$ = MKNODE(NODE_OBJECT_LITERAL, NULL, $2, NULL); }
    ;

field_list_opt:
    /* empty */ { $$ = NULL; }
    | field_list { $$ = $1; }
    ;

field_list:
    field_item { $$ = $1; }
    | field_list field_item {
        if ($2) {
            if ($1) { add_next($1, $2); $$ = $1; }
            else $$ = $2;
        } else {
            $$ = $1;
        }
    }
    ;

field_item:
    field { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    | COMMA { $$ = NULL; }
    | NEWLINE { $$ = NULL; }
    ;

field:
    name_token COLON expression { $$ = MKNODE(NODE_FIELD, $1, $3, NULL); }
    | name_token { $$ = MKNODE(NODE_FIELD, $1, NULL, NULL); }
    ;

name_token:
    IDENTIFIER { $$ = $1; }
    | TYPE { $$ = $1; }
    | INSTANCE { $$ = $1; }
    ;

stmt_end_opt:
    /* empty */ { $$ = NULL; }
    | SEMICOLON { $$ = NULL; }
    | NEWLINE { $$ = NULL; }
    ;

%%

extern char* yytext;

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d, column %d: %s (near '%s')\n", current_line, current_column, s, yytext);
}
