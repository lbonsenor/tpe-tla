#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "LangTeXAST.h"
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

 /* Latex semantic actions. */

 Program * ContentProgramSemanticAction(CompilerState * compilerState, Content * content);
 Content * AppendContentSemanticAction(Element * element, Content * content);
//  Content * SingleContentSemanticAction(Element * element);
//  Command * SimpleCommandSemanticAction(char * command);
 Command * ParameterizedCommandSemanticAction(char * command, ContentList * commandArgs);
 Command * EnvironmentCommandSemanticAction(Text * text, Content * params, ContentList * args, Content * content, Text * text2);
 Text * TextSemanticAction(char * text);
 Element * TextElementSemanticAction(Text * text);
 Element * CommandElementSemanticAction(Command * command);


  /* Langtex semantic actions. */

Element * LangtexCommandSemanticAction(LangtexCommand * command);
Element * LangtexCommandElementSemanticAction(LangtexCommand * langtexCommand);

  /* Langtex semantic actions - generic */

LangtexCommand * LangtexSimpleContentSemanticAction(LangtexParamList * parameters, Content * content, LangtexCommandType type);
LangtexCommand * LangtexCommandListSemanticAction(LangtexParamList * parameters, LangtexCommandList * commandList, LangtexCommandType type);
LangtexCommand * LangtexContentListSemanticAction(LangtexParamList * parameters, ContentList * contentList,LangtexCommandType type);

  /* Langtex semantic actions - specific */

LangtexCommand * TranslateSemanticAction(LangtexParamList * parameters, Text * leftContent, Text * rightContent);
LangtexCommand * DialogSemanticAction(LangtexParamList * parameters, LangtexCommandList * langtexCommandList);
LangtexCommand * ExerciseSemanticAction(LangtexParamList * parameters, LangtexCommand * commandPrompt, LangtexCommand *commandOptions, LangtexCommand * commandAnswers, LangtexCommandType type);
LangtexCommand * LanguageSemanticAction(TextList * textList, LangtexCommandType type);
LangtexCommand * FillSemanticAction(Text * text, LangtexCommandType type);

/* Langtex Parameter Type Actions */

LangtexParam * IntegerParamSemanticAction(char * key, int value);
LangtexParam * StringParamSemanticAction(char * key, char * value);
LangtexParam * BooleanParamSemanticAction(char * key, boolean value);

/* Langtex Parameter Actions */

LangtexParamList * SingleParam(LangtexParam * param);
LangtexParamList * AppendParam(LangtexParam * param, LangtexParamList * list);
LangtexParamList * EmptyParamList(void);

/* Utils */
LangtexCommandList * AppendLangtexComand(LangtexCommand * langtexCommand, LangtexCommandList * langtexCommandList);
LangtexCommandList * SingleLangtexCommand(LangtexCommand * command);
ContentList * ContentListSemanticAction(Content * content, ContentList * next);
TextList * TextListSemanticAction(Text * text, TextList * next);

#endif
