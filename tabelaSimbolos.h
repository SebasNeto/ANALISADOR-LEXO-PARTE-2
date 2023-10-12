// symbol_table.h

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char* identifier;
    int type;
    struct Symbol* proximo; // Aponta para o próximo símbolo na lista
} Symbol;


#define TABLE_SIZE 100
void iniciarTabela();
unsigned int hash(char* key);
// Protótipos das funções
void inserirSimbolo(char* identifier, int type);
Symbol* retornaSimbolo(char* identifier);
void removerSimbolo(char* identifier);


void print_simboloTabela() ;
#endif // SYMBOL_TABLE_H
