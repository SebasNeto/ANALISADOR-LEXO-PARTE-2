// symbol_table.c

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
    new_symbol->identifier = strdup(identifier);
    new_symbol->type = type;
    new_symbol->next = NULL;

    // Se o slot estiver vazio, insira diretamente
    if (symbol_table[slot] == NULL) {
        symbol_table[slot] = new_symbol;
    } else {
        // Caso contrário, anexe ao final da lista no slot
        Symbol* current = symbol_table[slot];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_symbol;
    }
}


Symbol* find_symbol(char* identifier) {
    unsigned int slot = hash(identifier);
    Symbol* current = symbol_table[slot];
    while (current != NULL) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
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
        Symbol* current = symbol_table[i];
        while (current != NULL) {
            printf("Slot %d: ID = %s, Tipo = %d\n", i, current->identifier, current->type);
            current = current->next;
        }
    }
}
