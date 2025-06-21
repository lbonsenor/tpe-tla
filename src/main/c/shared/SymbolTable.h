#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../shared/Type.h"
#include "../shared/Logger.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
    SYMBOL_LANGUAGE,     // A declared language (e.g., "ko", "chinese")
} SymbolType;

/**
 * A single symbol entry
 */
typedef struct SymbolEntry {
    char* name;                    // Symbol name (e.g., "ko" for language, or unique ID for translation) or i could i just autogenerate an id which would make more sense?
    SymbolType type;               // What kind of symbol
    
    // Data specific to symbol type
    union {
        // For SYMBOL_LANGUAGE
        struct {
            char* languageCode;    // e.g., "ko", "zh"
        } language;
        
    } data;
    
    struct SymbolEntry* next;      // Linked list pointer
} SymbolEntry;

/**
 * The symbol table itself
 */
typedef struct SymbolTable {
    SymbolEntry* head;             // First symbol in the list
    int languageCount;             // How many languages are declared
} SymbolTable;
/* BASIC SYMBOL TABLE OPERATIONS */

/**
 * Create a new empty symbol table
 */
SymbolTable* createSymbolTable(void);

/**
 * Destroy the symbol table and free all memory
 */
void destroySymbolTable(SymbolTable* table);

/**
 * Print the symbol table (for debugging)
 */
void printSymbolTable(SymbolTable* table);

/* LANGUAGE OPERATIONS */

/**
 * Add a language to the symbol table
 * Returns true if successful, false if language already exists
 */
boolean addLanguage(SymbolTable* table, const char* languageCode);

/**
 * Check if a language is declared in the symbol table
 */
boolean isLanguageDeclared(SymbolTable* table, const char* languageCode);

/**
 * Mark a language as used (when it appears in a translate command)
 */
/**
 * Get all declared languages
 */
char** getDeclaredLanguages(SymbolTable* table, int* count);

/**
 * Check for unused languages (declared but never used in translate)
 */
/* UTILITY FUNCTIONS */

/**
 * Clear all symbols from the table
 */
void clearSymbolTable(SymbolTable* table);

/**
 * Get statistics about the symbol table
 */
void getSymbolTableStats(SymbolTable* table, int* languageCount);
#endif