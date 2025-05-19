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

 Program * ContentProgramSemanticAction(CompilerState * compilerState, Content * content);
 Content * AppendContentSemanticAction(Element * element, Content * content);
 Content * SingleContentSemanticAction(Element * element);
 Command * SimpleCommandSemanticAction(char * command);
 Command * ParameterizedCommandSemanticAction(char * command, ContentList * commandArgs);
 Command * EnvironmentCommandSemanticAction(Text * text, Content * content, Text * text2);
 Text * TextSemanticAction(char * text);

 Element * CommandElementSemanticAction(Command * command);
 Element * TextElementSemanticAction(Text * text);
 Element * LangtexCommandSemanticAction(LangtexCommand *command);
 Element * LangtexCommandElementSemanticAction(LangtexCommand *langtexCommand);
 LangtexCommand * TranslateSemanticAction(LangtexParamList * parameters, Content *leftContent, Content *rightContent);
//  LangtexCommand * SpeakerSemanticAction(Text * leftText, Content *rightContent);
LangtexCommand * DialogSemanticAction(LangtexParamList * parameters, LangtexCommandList * langtexCommandList);
LangtexCommand * TableSemanticAction(LangtexParamList * parameters, LangtexCommandList * commandList, LangtexCommandType type);
LangtexCommand * RowSemanticAction(LangtexParamList * parameters, ContentList * contentList,LangtexCommandType type);
LangtexCommand * SpeakerSemanticAction(LangtexParamList * parameters, Content * content, LangtexCommandType type);
// LangtexCommandList * PromptSemanticAction(LangtexParamList * parameters,Content * content, LangtexCommandType type);
LangtexCommand * ExerciseSemanticAction(LangtexParamList * parameters, LangtexCommand * commandPrompt, LangtexCommand *commandOptions, LangtexCommand * commandAnswers, LangtexCommandType type);

LangtexParamList * SingleParam(LangtexParam * param);
LangtexParamList * AppendParam(LangtexParam * param, LangtexParamList * list);
LangtexParamList * EmptyParamList(void);


LangtexParam * IntegerParamSemanticAction(char * key, int value);
LangtexParam * StringParamSemanticAction(char * key, char * value);
LangtexParam * BooleanParamSemanticAction(char * key, boolean value);

// Utils
LangtexCommandList * AppendLangtexComand(LangtexCommand * langtexCommand, LangtexCommandList * langtexCommandList);
ContentList * ContentListSemanticAction(Content * content, ContentList * next);
LangtexCommandList * SingleLangtexCommand(LangtexCommand * command);
#endif
