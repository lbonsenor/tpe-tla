#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program * ContentProgramSemanticAction(CompilerState * compilerState, Content * content){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->content = content;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}
 Content * AppendContentSemanticAction(Element * element, Content * content){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Content * newContent = calloc(1, sizeof(Content));
	newContent->sequenceElement = element;
	newContent->sequenceContent = content;
	newContent->type = SEQUENCE;
	
	return newContent;
 }

 Content * SingleContentSemanticAction(Element * element){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Content * newContent = calloc(1, sizeof(Content));
	newContent->element = element;
	newContent->type = ELEMENT;
	return newContent;

 }

 Command * SimpleCommandSemanticAction(char * command){
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Command * newCommand = calloc(1, sizeof(Command));
    if (command != NULL) {
        newCommand->simpleCommand = command;
    } else {
        newCommand->simpleCommand = NULL;
        logError(_logger, "SimpleCommandSemanticAction received NULL command");
    }
    newCommand->type = SIMPLE;
    return newCommand;
 }

 Command * ParameterizedCommandSemanticAction(char * command, Content * content){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	// printf("Hello I'm parameterized\n"); <TODO change this
	logDebugging(_logger, "Matched parameterized command");
	Command * newCommand = calloc(1, sizeof(Command));
	// TODO: sacar esto 
	// if (command != NULL)
	// {
	// 	newCommand->parameterizedCommand = command;
	// }
	if (!command || !content) {
		logError(_logger, "ParameterizedCommandSemanticAction received NULL argument(s)");
		return NULL;
	}
	newCommand->parameterizedContent = content;
	newCommand->type = PARAMETERIZED;
	return newCommand;
 } 
 
 Command * EnvironmentCommandSemanticAction(Text * text, Content * content, Text * text2){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	// TODO: agregar esto para validar asi no tira segmenetacion fault si algo falta 
	if (!text || !content || !text2) {
		logError(_logger, "EnvironmentCommandSemanticAction received NULL argument(s)");
		return NULL;
	}
	if (strcmp(text->text, text2->text) != 0) {
		logError(_logger, "Mismatched environment names: %s ≠ %s", text->text, text2->text);
		currentCompilerState()->succeed = false;
		// Could maybe set compilerState->succeed = false
		return NULL;
	}
	Command * newCommand = calloc(1, sizeof(Command));
	newCommand->environmentLeftText = text;
	newCommand->environmentContent = content;
	newCommand->environmentRightText = text2;
	newCommand->type = ENVIRONMENT;
	return newCommand;
 }

 Text * TextSemanticAction(char * text){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Text * newText = calloc(1, sizeof(Text));
	newText->text = text;
	return newText;
 }

 Element * CommandElementSemanticAction(Command *command)
 {
    _logSyntacticAnalyzerAction(__FUNCTION__);
	// TODO:
	if (!command) {
		logError(_logger, "CommandElementSemanticAction received NULL command");
		return NULL;
	}
	Element * newElement = calloc(1, sizeof(Element));
	newElement->command = command;
	newElement->type = LATEX_COMMAND;
	return newElement;
 }

 Element *TextElementSemanticAction(Text *text)
 {
    _logSyntacticAnalyzerAction(__FUNCTION__);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->text = text;
	newElement->type = LATEX_TEXT;
	return newElement;
 }


  Element * LangtexCommandElementSemanticAction(LangtexCommand *langtexCommand)
 {
    _logSyntacticAnalyzerAction(__FUNCTION__);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->langtexCommand = langtexCommand;
	newElement->type = LANGTEX_COMMAND;
	return newElement;
 }

 LangtexCommand * TranslateSemanticAction(Text *leftText, Text *rightText)
 {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	// if (!contentBefore || !contentAfter) {
	// 	logError(_logger, "TranslateSemanticAction received NULL parameter(s)");
	// 	return NULL;
	// }
	langtexCommand->leftText=leftText;
	langtexCommand->rightText=rightText;
	langtexCommand->type = LANGTEX_TRANSLATE;
	return langtexCommand;
 }

