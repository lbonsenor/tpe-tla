#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "LaTeX.h"
// #include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

 Program * ContentProgramSemanticAction(CompilerState * compilerState, Content * content);
 Content * AppendContentSemanticAction(Element * element, Content * content);
 Content * SingleContentSemanticAction(Element * element);
 Command * SimpleCommandSemanticAction(char * command);
 Command * ParameterizedCommandSemanticAction(char * command, Content * content); 
 Command * EnvironmentCommandSemanticAction(Text * text, Content * content, Text * text2);
 Text * TextSemanticAction(char * text);

 Element * CommandElementSemanticAction(Command * command);
 Element * TextElementSemanticAction(Text * text);

#endif
