#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "Type.h"

/**
 * Forward declarations
 */
typedef struct SymbolTable SymbolTable;
typedef struct ScopeStack ScopeStack;
typedef struct ErrorList ErrorList;
typedef struct WarningList WarningList;

typedef enum {
    PHASE_LEXICAL,
    PHASE_SYNTACTIC, 
    PHASE_SEMANTIC,
    PHASE_CODE_GENERATION
} CompilationPhase;


// /**
//  * Configuration for LangTeX compilation
//  */
// typedef struct {
//     char* defaultLanguage;      // Default source language
//     char* outputFormat;         // Just .tex tbh
//     boolean enableWarnings;     // Enable warning generation
//     boolean strictMode;         // Strict semantic checking
//     int maxErrors;              // Maximum errors before stopping
// } LangTexConfig;

// /**
//  * Context stack for nested language constructs
//  */
// typedef enum {
//     CONTEXT_GLOBAL,
//     CONTEXT_DIALOG,
//     CONTEXT_EXERCISE,
//     CONTEXT_TABLE,
//     CONTEXT_TRANSLATE,
//     CONTEXT_BLOCK
// } ContextType;

// typedef struct ContextFrame {
//     ContextType type;
//     void* data;                 // Context-specific data
//     struct ContextFrame* parent;
// } ContextFrame;

// /**
//  * Error and warning management
//  */
// typedef struct ErrorEntry {
//     int line;
//     int column;
//     char* message;
//     struct ErrorEntry* next;
// } ErrorEntry;

// typedef struct {
//     ErrorEntry* head;
//     ErrorEntry* tail;
//     int count;
// } ErrorList;

// typedef struct {
//     ErrorEntry* head;
//     ErrorEntry* tail;
//     int count;
// } WarningList;

/**
 * The general status of a compilation.
*/
typedef enum {
    SUCCEED = 0,
    FAILED = 1,
    FAILED_WITH_WARNINGS = 2
} CompilationStatus;

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	// The root node of the AST.
	void * abstractSyntaxtTree;

	// A flag that indicates the current state of the compilation so far.
	boolean succeed;

    // === Symbol Management ===
    SymbolTable* globalSymbolTable;    // Global symbols (languages, etc.)
    // ScopeStack* scopeStack;             // Stack for nested scopes

	// // === Context Management ===
    // ContextFrame* contextStack;        // Stack of nested contexts

	// // === Configuration ===
    // LangTexConfig* config;             // Compilation configuration

	// // === Error and Warning Management ===
    // ErrorList* errors;                 // Compilation errors
    // WarningList* warnings;             // Compilation warnings
    // int maxErrorsReached;              // Flag if max errors exceeded
    
    // // === Semantic Analysis State ===
    // SymbolTable* currentScope;         // Current lexical scope
    // boolean inTranslateBlock;          // Are we in a translate command? TODO: why is it only in translate?
    // char* currentLanguage;             // Current language context
    
    // // === Statistics ===
    // int lineCount;                     // Total lines processed
    // int tokenCount;                    // Total tokens processed
    
    // // === Output Management ===
    // char* outputBuffer;                // Generated output buffer
    // size_t outputSize;                 // Size of output buffer
    // size_t outputCapacity;             // Capacity of output buffer

	CompilationPhase currentPhase;      // Current compilation phase
    int errorCount;                     // Count of semantic errors
    int warningCount;                   // Count of semantic warnings
    boolean semanticAnalysisComplete;   // Flag for semantic analysis completion

	// TODO: Add an stack to handle nested scopes.
	// TODO: Add symbols stack.
	// TODO: Add configuration.
	// TODO: ...

	// The computed value of the entire program (only for the calculator).
	// int value;
} CompilerState;

// === CompilerState Management Functions ===

/**
 * Initialize a new compiler state
 */
CompilerState* createCompilerState();

/**
 * Destroy compiler state and free all resources
 */
void destroyCompilerState(CompilerState* state);

/**
 * Reset compiler state for new compilation
 */
void resetCompilerState(CompilerState* state);

// === Context Stack Management ===

/**
 * Push a new context onto the stack
 */
// void pushContext(CompilerState* state, ContextType type, void* data);

/**
 * Pop the top context from the stack
 */
void popContext(CompilerState* state);

/**
 * Get the current context type
 */
// ContextType getCurrentContextType(CompilerState* state);

/**
 * Check if we're in a specific context
 */
// boolean isInContext(CompilerState* state, ContextType type);

// === Error and Warning Management ===

/**
 * Add an error to the compilation state
 */
void addError(CompilerState* state, int line, int column, const char* format, ...);

/**
 * Add a warning to the compilation state
 */
void addWarning(CompilerState* state, int line, int column, const char* format, ...);

/**
 * Check if compilation should stop due to errors
 */
boolean shouldStopCompilation(CompilerState* state);

/**
 * Print all errors and warnings
 */
void printDiagnostics(CompilerState* state);

// === Scope Management ===

/**
 * Enter a new lexical scope
 */
void enterScope(CompilerState* state);

/**
 * Exit the current lexical scope
 */
void exitScope(CompilerState* state);

/**
 * Declare a symbol in the current scope
 */
boolean declareSymbol(CompilerState* state, const char* name, void* symbolData);

/**
 * Lookup a symbol in the current scope chain
 */
void* lookupSymbol(CompilerState* state, const char* name);

#endif
