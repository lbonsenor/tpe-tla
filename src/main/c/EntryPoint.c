#include "backend/code-generation/Generator.h"
#include "backend/domain-specific/SemanticAnalyzer.h"
// #include "backend/domain-specific/Calculator.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/LangTeXAST.h"
// #include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
const int main(const int count, const char **arguments)
{
	Logger *logger = createLogger("EntryPoint");
	// initializeSymbolTableModule();
	initializeFlexActionsModule();
	initializeBisonActionsModule();
	initializeSyntacticAnalyzerModule();
	initializeRenameMeModule();
	// initializeCalculatorModule();
	// initializeLangtexModule();
	// initializeGeneratorModule();

	// Logs the arguments of the application.
	for (int k = 0; k < count; ++k)
	{
		logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
	}

	// Begin compilation process.
	CompilerState compilerState = {
		.abstractSyntaxtTree = NULL,
		.succeed = false,
		.globalSymbolTable = createSymbolTable(),
		// .config = NULL,
		// .inDialogContext = false,
		// .inExerciseContext = false,
		// .inTableContext = false,
		.warningCount = 0,
		// NEW: Initialize semantic analysis fields
		.currentPhase = PHASE_LEXICAL,
		.errorCount = 0,
		.semanticAnalysisComplete = false};

	initializeSemanticAnalyzer(&compilerState);

	const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);
	CompilationStatus compilationStatus = SUCCEED;
	Program *program = compilerState.abstractSyntaxtTree;
	if (syntacticAnalysisStatus == ACCEPT)
	{
		// ----------------------------------------------------------------------------------------
		// Beginning of the Backend... ------------------------------------------------------------
		logDebugging(logger, "Validating the input program...");
		// ComputationResult computationResult = computeContent(program->content);
		// if (computationResult.succeed) {
		// 	compilerState.value = computationResult.value;
		// 	generate(&compilerState);
		// }

		SemanticAnalysisStatus semanticResult = analyzeProgram(&compilerState, program);
		if (semanticResult == SEMANTIC_ANALYSIS_ACCEPT)
		{
			// generate(&compilerState);
		}
		else
		{
			logError(logger, "The computation phase rejects the input program.");
			compilationStatus = FAILED;
		}
		// ...end of the Backend. -----------------------------------------------------------------
		// ----------------------------------------------------------------------------------------
	}
	else
	{
		logError(logger, "The syntactic-analysis phase rejects the input program.");
		compilationStatus = FAILED;
	}

	if (compilerState.globalSymbolTable)
	{
		int langCount, translationCount, usedLangCount;
		getSymbolTableStats(compilerState.globalSymbolTable, &langCount);

		logDebugging(logger, "=== SEMANTIC ANALYSIS RESULTS ===");
		logDebugging(logger, "Languages found: %d", langCount);

		// Print full symbol table

		printf("\n");
		printSymbolTable(compilerState.globalSymbolTable);
		logDebugging(logger, "================================");
	}

	logDebugging(logger, "Releasing AST resources...");
	releaseProgram(program);
	logDebugging(logger, "Releasing modules resources...");
	if (compilerState.globalSymbolTable)
	{
		destroySymbolTable(compilerState.globalSymbolTable);
	}
	// shutdownGeneratorModule();
	// shutdownLangtexModule();
	// shutdownCalculatorModule();
	shutdownSemanticAnalyzer();
	shutdownRenameMeModule();
	shutdownSyntacticAnalyzerModule();
	shutdownBisonActionsModule();
	shutdownFlexActionsModule();
	// shutdownSymbolTableModule();
	logDebugging(logger, "Compilation is done.");
	destroyLogger(logger);
	return compilationStatus;
}

// TODO: this should be in CompilerState.c
void addSemanticError(CompilerState *state, const char *message)
{
	if (state)
	{
		state->errorCount++;
		state->succeed = false;
		// You can expand this to store actual error messages later
		printf("Semantic Error: %s\n", message);
	}
}

void addSemanticWarning(CompilerState *state, const char *message)
{
	if (state)
	{
		state->warningCount++;
		// You can expand this to store actual warning messages later
		printf("Semantic Warning: %s\n", message);
	}
}

boolean hasSemanticErrors(CompilerState *state)
{
	return state ? state->errorCount > 0 : true;
}