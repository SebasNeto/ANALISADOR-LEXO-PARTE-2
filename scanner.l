%option yylineno
%option noyywrap

%{

#include "parser.tab.h"
#include "tabelaSimbolos.h"
#include <stdio.h>
void yyerror(const char *s);

%}

%%

"int"                { printf("TOKEN: KW_INT\n"); return KW_INT; }
"real"               { printf("TOKEN: KW_REAL\n"); return KW_REAL; }
"void"               { printf("TOKEN: VOID\n"); return VOID; }
"if"                 { printf("TOKEN: IF\n"); return IF; }
"else"               { printf("TOKEN: ELSE\n"); return ELSE; }
"while"              { printf("TOKEN: WHILE\n"); return WHILE; }
"loop"  { printf("TOKEN: LOOP\n"); return LOOP; }
"input" { printf("TOKEN: INPUT\n"); return INPUT; }

"return"             { printf("TOKEN: KW_RETURN\n"); return RETURN; }

"=="                 { printf("TOKEN: EQ\n"); return EQ; }
"<="                 { printf("TOKEN: LEQ\n"); return LEQ; }
"<"                  { printf("TOKEN: LT\n"); return LT; }
">"                  { printf("TOKEN: GT\n"); return GT; }
">="                 { printf("TOKEN: GEQ\n"); return GEQ; }
"!="                 { printf("TOKEN: NEQ\n");return NEQ; }

"+"|"-"|"*"|"/"|"<"|"="|";"|","|"("|")"|"["|"]"|"{"|"}"|">"|"!"|"&" {printf("TOKEN: %c\n", yytext[0]);  return yytext[0]; }


[0-9]+               { printf("TOKEN: LIT_INT %s\n", yytext); return LIT_INT; }
[0-9]+[.][0-9]+      { printf("TOKEN: LIT_REAL %s\n", yytext); return LIT_REAL; }
'.'                  { printf("TOKEN: LIT_CHAR %s\n", yytext); return LIT_CHAR; }

[a-zA-Z_][a-zA-Z_0-9]* {
    printf("ID: %s\n", yytext);
    yylval.strValue = strdup(yytext);
    return ID;
}


"//"[^"\n"]*          { /* Ignora comentários de uma linha */ }
"/*"(.|\n)*?"*/"     { /* Ignora comentários de várias linhas */ }

[ \t\r\n]+           /* Ignora espaços em branco e quebras de linha */
.                    { yyerror("Caractere inválido"); }

%%

