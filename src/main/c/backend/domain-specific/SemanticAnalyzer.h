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
void initializeSemanticAnalyzer();
void shutdownSemanticAnalyzer();

/* MAIN ANALYSIS FUNCTIONS */
SemanticAnalysisStatus analyzeProgram(Program* program);
SemanticAnalysisStatus analyzeContent(Content* content);
SemanticAnalysisStatus analyzeElement(Element* element);
SemanticAnalysisStatus analyzeLangtexCommand(LangtexCommand* command);
SemanticAnalysisStatus analyzeTranslateCommand(LangtexCommand* command);
SemanticAnalysisStatus analyzeTableCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeRowCommand(LangtexCommand *command, int expectedCols);
SemanticAnalysisStatus analyzeDialogCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeSpeakerCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeBlockCommand(LangtexCommand *command);
SemanticAnalysisStatus analyzeExerciseCommand(LangtexCommand *command);


/* HELPER FUNCTIONS */
LangtexParam* getParameter(LangtexParamList* params, const char* name);
static boolean validateLatexInCommand(Command *command);
static boolean validateLatexInElement(Element *element);
static boolean validateLatexInContent(Content *content);
static boolean validateLangtexContent(Content *content);
static boolean validateLangtexElement(Element *element);
static boolean validatePromptContent(Content *content);
static boolean validateSingleChoiceExercise(LangtexCommand *prompt, LangtexCommand *answer);
static boolean validateMultipleChoiceExercise(LangtexCommand *prompt, LangtexCommand *options, LangtexCommand *answers);
#endif