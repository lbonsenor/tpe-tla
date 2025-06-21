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
void initializeSemanticAnalyzer();

/**
 * Shutdown the semantic analyzer
 */
void shutdownSemanticAnalyzer();

/* MAIN ANALYSIS FUNCTIONS */

/**
 * Analyze a complete program
 */
SemanticAnalysisStatus analyzeProgram(Program* program);

/**
 * Analyze program content
 */
SemanticAnalysisStatus analyzeContent(Content* content);

/**
 * Analyze an element
 */
SemanticAnalysisStatus analyzeElement(Element* element);

/**
 * Analyze a LangTeX command
 */
SemanticAnalysisStatus analyzeLangtexCommand(LangtexCommand* command);

/**
 * Analyze a LaTeX command (stub for now)
 */
SemanticAnalysisStatus analyzeLatexCommand(Command* command);

/* TRANSLATE-SPECIFIC ANALYSIS */

/**
 * Analyze [!translate] command specifically
 */
SemanticAnalysisStatus analyzeTranslateCommand(LangtexCommand* command);

SemanticAnalysisStatus analyzeTableCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeRowCommand(LangtexCommand *command, int expectedCols);
SemanticAnalysisStatus analyzeDialogCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeSpeakerCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeBlockCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeExerciseCommand(LangtexCommand *command);


/* PARAMETER HELPERS */

/**
 * Get parameter by name from parameter list
 */
LangtexParam* getParameter(LangtexParamList* params, const char* name);

/* STUB FUNCTIONS - Required by interface but not implemented yet */
boolean validateParameters(LangtexParamList* params, LangtexCommandType expectedFor);
boolean validateParameter(LangtexParam* param, const char* expectedName, LangtexParamType expectedType);
boolean validateContext(LangtexCommandType commandType);
boolean checkNestingRules(LangtexCommandType commandType);
static boolean validateLatexInCommand(Command *command);
static boolean validateLatexInElement(Element *element);
static boolean validateLatexInContent(Content *content);
static boolean validateSpeakerContent(Content *content);
static boolean validateSpeakerElement(Element *element);
static boolean validatePromptContent(Content *content);
static boolean validateSingleChoiceExercise(LangtexCommand *prompt, LangtexCommand *answer);
static boolean validateMultipleChoiceExercise(LangtexCommand *prompt, LangtexCommand *options, LangtexCommand *answers);
#endif