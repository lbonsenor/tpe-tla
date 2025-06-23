#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;


/* BASIC SYMBOL TABLE OPERATIONS */

SymbolTable* createSymbolTable(void) {
    SymbolTable* table = calloc(1, sizeof(SymbolTable));
    if (!table) {
        return NULL;
    }
    
    table->head = NULL;
    table->languageCount = 0;    
    return table;
}

void destroySymbolTable(SymbolTable* table) {
    if (!table) {
        return;
    }
    
    SymbolEntry* current = table->head;
    while (current) {
        SymbolEntry* next = current->next;
        
        // Free the name
        if (current->name) {
            free(current->name);
        }
        
        // Free type-specific data
        switch (current->type) {
            case SYMBOL_LANGUAGE:
                if (current->data.language.languageCode) {
                    free(current->data.language.languageCode);
                }
                break;
        }
        
        free(current);
        current = next;
    }
    
    free(table);
}

void printSymbolTable(SymbolTable* table) {
    if (!table) {
        printf("Symbol table is NULL\n");
        return;
    }
    
    printf("\n=== SYMBOL TABLE ===\n");
    printf("Languages: %d, \n\n", table->languageCount);
    
    // Print languages first
    printf("LANGUAGES:\n");
    SymbolEntry* current = table->head;
    while (current) {
        if (current->type == SYMBOL_LANGUAGE) {
            printf("  %s - %s\n", 
                   current->data.language.languageCode,
                   "Param Langugae");
        }
        current = current->next;
    }
    
    printf("==================\n\n");
}

/* LANGUAGE OPERATIONS */

boolean addLanguage(SymbolTable* table, const char* languageCode) {
    if (!table || !languageCode) {
        return false;
    }
    
    // Check if language already exists
    if (isLanguageDeclared(table, languageCode)) {
        return false; // Already exists
    }
    
    // Create new language entry
    SymbolEntry* entry = calloc(1, sizeof(SymbolEntry));
    if (!entry) {
        return false;
    }
    
    entry->name = strdup(languageCode);  // Use language code as the name
    entry->type = SYMBOL_LANGUAGE;
    entry->data.language.languageCode = strdup(languageCode);
    entry->next = table->head;
    
    table->head = entry;
    table->languageCount++;
    
    return true;
}

boolean isLanguageDeclared(SymbolTable* table, const char* languageCode) {
    if (!table || !languageCode) {
        return false;
    }
    
    SymbolEntry* current = table->head;
    while (current) {
        if (current->type == SYMBOL_LANGUAGE && 
            strcmp(current->data.language.languageCode, languageCode) == 0) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

char** getDeclaredLanguages(SymbolTable* table, int* count) {
    if (!table || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    if (table->languageCount == 0) {
        *count = 0;
        return NULL;
    }
    
    char** languages = malloc(table->languageCount * sizeof(char*));
    if (!languages) {
        *count = 0;
        return NULL;
    }
    
    int index = 0;
    SymbolEntry* current = table->head;
    while (current && index < table->languageCount) {
        if (current->type == SYMBOL_LANGUAGE) {
            languages[index] = strdup(current->data.language.languageCode);
            index++;
        }
        current = current->next;
    }
    
    *count = index;
    return languages;
}


/* UTILITY FUNCTIONS */

void clearSymbolTable(SymbolTable* table) {
    if (!table) {
        return;
    }
    
    SymbolEntry* current = table->head;
    while (current) {
        SymbolEntry* next = current->next;
        
        // Free memory for this entry (same as in destroySymbolTable)
        if (current->name) {
            free(current->name);
        }
        
        switch (current->type) {
            case SYMBOL_LANGUAGE:
                if (current->data.language.languageCode) {
                    free(current->data.language.languageCode);
                }
                break;
        }
        
        free(current);
        current = next;
    }
    
    table->head = NULL;
    table->languageCount = 0;
}

void getSymbolTableStats(SymbolTable* table, int* languageCount) {
    if (!table) {
        if (languageCount) *languageCount = 0;
        return;
    }
    
    if (languageCount) {
        *languageCount = table->languageCount;
    }
    
}