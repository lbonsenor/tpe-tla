#include "backend/code-generation/Generator.h"
#include "backend/domain-specific/SemanticAnalyzer.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/LangTeXAST.h"
// #include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"
#include <getopt.h>
#include <string.h>

/**i
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
const int main(const int count, char **arguments)
{
	Logger *logger = createLogger("EntryPoint");
	initializeFlexActionsModule();
	initializeBisonActionsModule();
	initializeSyntacticAnalyzerModule();
	initializeRenameMeModule();
	initializeSemanticAnalyzer();
	initializeGeneratorModule();

	int opt;
    bool input_flag = false;
    char *output_path;

	for (int i = 0; i < count; i++)
	{
		logInformation(logger, "Argument %d: %s", i, arguments[i]);
	}
	

	// Logs the arguments of the application.
	    struct option long_options[] = {
        {"input", no_argument, 0, 'i'},
        {0, 0, 0, 0}
    };

    while (count > 1 && (opt = getopt_long(count, arguments, "o:i", long_options, NULL)) != -1) {
        switch (opt) {
            case 'o':
                output_path = optarg;
				logDebugging(logger, "Output Path is %s", output_path);
                break;
            case 'i':
                input_flag = true;
				logDebugging(logger, "Compiling in input mode (no prologue)");
                break;
            default:
                break;
        }
    }

	// Begin compilation process.
	CompilerState compilerState = {
		.abstractSyntaxtTree = NULL,
		.succeed = false
	};


	const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);
	CompilationStatus compilationStatus = SUCCEED;
	Program *program = compilerState.abstractSyntaxtTree;
	if (syntacticAnalysisStatus == ACCEPT)
	{
		// ----------------------------------------------------------------------------------------
		// Beginning of the Backend... ------------------------------------------------------------
		logDebugging(logger, "Validating the input program...");

		SemanticAnalysisStatus semanticResult = analyzeProgram(program);
		if (semanticResult == SEMANTIC_ANALYSIS_ACCEPT)
		{
			generate(output_path, input_flag, &compilerState);
		}
		else
		{
			logError(logger, "The semantic analysis phase rejects the input program.");
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

	logDebugging(logger, "Releasing AST resources...");
	releaseProgram(program);
	logDebugging(logger, "Releasing modules resources...");

	shutdownGeneratorModule();
	shutdownSemanticAnalyzer();
	shutdownRenameMeModule();
	shutdownSyntacticAnalyzerModule();
	shutdownBisonActionsModule();
	shutdownFlexActionsModule();
	logDebugging(logger, "Compilation is done.");
	destroyLogger(logger);
	return compilationStatus;
}
