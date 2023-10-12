#include "tabelaSimbolos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Symbol* symbol_table[TABLE_SIZE];

void init_symbol_table() {
    for(int i = 0; i < TABLE_SIZE; i++) {
        symbol_table[i] = NULL;
    }
}

unsigned int hash(char* key) {
    unsigned int value = 0;
    for (char* p = key; *p != '\0'; p++) {
        value = value * 37 + *p;
    }
    return value % TABLE_SIZE;
}

void insert_symbol(char* identifier, int type) {
    unsigned int slot = hash(identifier);
    Symbol* new_symbol = (Symbol*) malloc(sizeof(Symbol));
    new_symbol->identifier = strdup(identifier); // Duplica a string
    new_symbol->type = type;

    if (symbol_table[slot] != NULL) {
        printf("Colisão detectada para: %s no slot %d\n", identifier, slot);
    }
    
    symbol_table[slot] = new_symbol; // Simples inserção para este exemplo. Pode haver colisões
    printf("Inserindo: %s no slot %d\n", identifier, slot);
}

Symbol* find_symbol(char* identifier) {
    unsigned int slot = hash(identifier);
    Symbol* current = symbol_table[slot];
    
    if (current && strcmp(current->identifier, identifier) == 0) {
        printf("Símbolo encontrado: %s no slot %d\n", identifier, slot);
        return current;
    } else {
        printf("Símbolo não encontrado: %s\n", identifier);
        return NULL;
    }
}

void remove_symbol(char* identifier) {
    unsigned int slot = hash(identifier);
    free(symbol_table[slot]->identifier);
    free(symbol_table[slot]);
    symbol_table[slot] = NULL;
}

void print_symbol_table() {
    printf("Tabela de Símbolos:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (symbol_table[i] != NULL) {
            printf("Slot %d: ID = %s, Tipo = %d\n", i, symbol_table[i]->identifier, symbol_table[i]->type);
            // Adicione mais detalhes se sua estrutura Symbol tiver mais campos
        }
    }
}

///////////////////////////////////////

%{

#include <stdio.h>
#include <stdlib.h>
#include "lex.yy.h"
#include "tabelaSimbolos.h"


extern int yylex();
extern char* yytext;
extern int yylineno;


int getLineNumber(void);
void yyerror(const char *s);

%}

%union {
    int intValue;
    char* strValue;

}

%token <strValue> ID
%type<intValue> espec_tipo
%type<strValue> var

%token INT VOID
%token LIT_INT LIT_REAL LIT_CHAR
%token NUM
%token LOOP
%token INPUT
%token IF ELSE WHILE RETURN
%token EQ LEQ LT GT GEQ NEQ PLUS MINUS TIMES DIV MOD
%token<intValue> KW_CHAR KW_INT KW_REAL
%start programa

%%

programa: lista_decl lista_com;

lista_decl: lista_decl decl
          | decl
          ;

decl: decl_var
    | decl_func
    ;

decl_var: espec_tipo var '=' init_val ';'
{
    Symbol* sym = find_symbol($2);
    if (sym) {
        sym->type = $1; 
    }else{
        insert_symbol($2,$1);
    }
}

init_val: LIT_INT
        | LIT_CHAR
        | LIT_REAL
        | '{' literal_list '}'
        ;

literal_list: literal_list LIT_INT
            | literal_list LIT_CHAR
            | literal_list LIT_REAL
            | LIT_INT
            | LIT_CHAR
            | LIT_REAL
            ;
decl_func: espec_tipo ID '(' params ')' com_comp
         ;

espec_tipo: KW_CHAR  { $$ = KW_CHAR; }
          | KW_INT   { $$ = KW_INT; }
          | KW_REAL  { $$ = KW_REAL; }
          ;


params: lista_param
      | VOID
      | /* vazio */
      ;

lista_param: lista_param ',' param
           | param
           ;

param: espec_tipo var;

decl_locais: decl_locais decl_var
           | /* vazio */
           ;

lista_com: comando lista_com
         | /* vazio */
         ;

comando: com_expr
       | com_atrib
       | com_comp
       | com_selecao
       | com_repeticao
       | com_retorno
       ;

com_expr: exp ';'
        | ';'
        ;

com_atrib: var '=' exp ';';

com_comp: '{' decl_locais lista_com '}';

com_selecao: IF '(' exp ')' comando
           | IF '(' exp ')' com_comp ELSE comando
           | IF '(' exp ')' LOOP comando
           ;

com_repeticao: WHILE '(' exp ')' comando ';';

com_retorno: RETURN ';'
           | RETURN exp ';'
           ;

exp: exp_soma op_relac exp_soma
   | exp_soma
   | INPUT '(' espec_tipo ')'
   | exp_simples
   ;

op_relac: LEQ
        | LT
        | GT
        | GEQ
        | EQ
        | NEQ
        ;

exp_soma: exp_soma op_soma exp_mult
        | exp_mult
        
        ;

op_soma: '+'
       | '-'
       ;

exp_mult: exp_mult op_mult exp_simples
        | exp_simples
        ;

op_mult: '*'
       | '/'
       | '%'
       ;

exp_simples: '(' exp ')'
           | var
           | cham_func
           | literais
           ;

literais: LIT_INT
        | LIT_REAL
        | LIT_CHAR
        ;

cham_func: ID '(' args ')';

var: ID { $$ = $1; }
   | ID '[' LIT_INT ']'
   ;

args: lista_arg
    | /* vazio */
    ;

lista_arg: lista_arg ',' exp
         | exp
         ;

%%

int getLineNumber(void) {
    return yylineno;
}

void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático na linha %d: %s\n", getLineNumber(), s);
    exit(3);
}
