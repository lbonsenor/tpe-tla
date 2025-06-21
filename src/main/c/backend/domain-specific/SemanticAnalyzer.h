#ifndef SEMANTIC_ANALYZER_HEADER
#define SEMANTIC_ANALYZER_HEADER

// TODO: change this to a domain-specific model instead of using the frontend AST directly
#include "../../frontend/syntactic-analysis/LangTeXAST.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/SymbolTable.h"
#include <stdlib.h>


/**
 * Semantic analysis status codes
 */
typedef enum {
    SEMANTIC_ANALYSIS_ACCEPT,
    SEMANTIC_ANALYSIS_REJECT,
    SEMANTIC_ANALYSIS_ERROR
} SemanticAnalysisStatus;

/* MODULE MANAGEMENT */

/**
 * Initialize the semantic analyzer
 */
boolean initializeSemanticAnalyzer(CompilerState* state);

/**
 * Shutdown the semantic analyzer
 */
void shutdownSemanticAnalyzer(void);

/* MAIN ANALYSIS FUNCTIONS */

/**
 * Analyze a complete program
 */
SemanticAnalysisStatus analyzeProgram(CompilerState* state, Program* program);

/**
 * Analyze program content
 */
SemanticAnalysisStatus analyzeContent(CompilerState* state, Content* content);

/**
 * Analyze an element
 */
SemanticAnalysisStatus analyzeElement(CompilerState* state, Element* element);

/**
 * Analyze a LangTeX command
 */
SemanticAnalysisStatus analyzeLangtexCommand(CompilerState* state, LangtexCommand* command);

/**
 * Analyze a LaTeX command (stub for now)
 */
SemanticAnalysisStatus analyzeLatexCommand(CompilerState* state, Command* command);

/* TRANSLATE-SPECIFIC ANALYSIS */

/**
 * Analyze [!translate] command specifically
 */
SemanticAnalysisStatus analyzeTranslateCommand(CompilerState* state, LangtexCommand* command);

SemanticAnalysisStatus analyzeTableCommand(CompilerState *state, LangtexCommand *command);
SemanticAnalysisStatus analyzeRowCommand(CompilerState *state, LangtexCommand *command, int expectedCols);
SemanticAnalysisStatus analyzeDialogCommand(CompilerState *state, LangtexCommand *command);
SemanticAnalysisStatus analyzeSpeakerCommand(CompilerState *state, LangtexCommand *command);
SemanticAnalysisStatus analyzeBlockCommand(CompilerState *state, LangtexCommand *command);
SemanticAnalysisStatus analyzeExerciseCommand(CompilerState *state, LangtexCommand *command);


/* PARAMETER HELPERS */

/**
 * Get parameter by name from parameter list
 */
LangtexParam* getParameter(LangtexParamList* params, const char* name);

/* STUB FUNCTIONS - Required by interface but not implemented yet */
boolean validateParameters(CompilerState* state, LangtexParamList* params, LangtexCommandType expectedFor);
boolean validateParameter(CompilerState* state, LangtexParam* param, const char* expectedName, LangtexParamType expectedType);
boolean validateContext(CompilerState* state, LangtexCommandType commandType);
boolean checkNestingRules(CompilerState* state, LangtexCommandType commandType);
static boolean validateLatexInCommand(Command *command);
static boolean validateLatexInElement(Element *element);
static boolean validateLatexInContent(Content *content);
static boolean validateSpeakerContent(CompilerState* state, Content *content);
static boolean validateSpeakerElement(CompilerState * state, Element *element);
static boolean validatePromptContent(CompilerState *state, Content *content);
static boolean validateSingleChoiceExercise(CompilerState *state, LangtexCommand *prompt, LangtexCommand *answer);
static boolean validateMultipleChoiceExercise(CompilerState *state, LangtexCommand *prompt, LangtexCommand *options, LangtexCommand *answers);
#endif