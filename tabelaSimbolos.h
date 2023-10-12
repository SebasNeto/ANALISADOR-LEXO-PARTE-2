// symbol_table.h

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char* identifier;
    int type;
    struct Symbol* next; // Aponta para o próximo símbolo na lista
} Symbol;


#define TABLE_SIZE 100
void init_symbol_table();
// Protótipos das funções
void insert_symbol(char* identifier, int type);
Symbol* find_symbol(char* identifier);
void remove_symbol(char* identifier);


void print_symbol_table();
#endif // SYMBOL_TABLE_H
