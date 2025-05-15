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
	printf("Entering Content Program: %s\n", content->type == SEQUENCE ? "SEQUENCE" : "SINGLE");
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
	printf("Entering Sequence Content\n");
	printf("Type %s: Command: %s\n", element->type == LATEX_COMMAND ? "Latex" : "Text", element->command == NULL ? "null" : "not null");
	Content * newContent = calloc(1, sizeof(Content));
	newContent->sequenceElement = element;
	newContent->sequenceContent = content;
	newContent->type = SEQUENCE;
	
	return newContent;
 }

 Content * SingleContentSemanticAction(Element * element){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	printf("Entering Single Content %s\n", element->type == LATEX_COMMAND ? "Command" : "Text");
	Content * newContent = calloc(1, sizeof(Content));
	newContent->element = element;
	newContent->type = ELEMENT;
	return newContent;

 }

 Command * SimpleCommandSemanticAction(char * command){
    _logSyntacticAnalyzerAction(__FUNCTION__);
	printf("Entering Simple Command: %s\n", command);
    Command * newCommand = calloc(1, sizeof(Command));
    if (command != NULL) {
        newCommand->simpleCommand = malloc(strlen(command) + 1);
		strcpy(newCommand->simpleCommand, command);
    } else {
        newCommand->simpleCommand = NULL;
        logError(_logger, "SimpleCommandSemanticAction received NULL command");
    }
    newCommand->type = SIMPLE;
    return newCommand;
 }

 Command * ParameterizedCommandSemanticAction(char * command, Content * content){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Command * newCommand = calloc(1, sizeof(Command));
	if (command != NULL)
	{
		newCommand->parameterizedCommand = malloc(strlen(command) + 1);
		strcpy(newCommand->parameterizedCommand, command);
	}
	newCommand->parameterizedContent = content;
	newCommand->type = PARAMETERIZED;
	return newCommand;
 } 
 
 Command * EnvironmentCommandSemanticAction(Text * text, Content * content, Text * text2){
	_logSyntacticAnalyzerAction(__FUNCTION__);
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
	printf("Entering Text\n");
	return newText;
 }

 Element * CommandElementSemanticAction(Command *command)
 {
    _logSyntacticAnalyzerAction(__FUNCTION__);
	printf("Entering Command Element: %s\n", command->simpleCommand);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->command = command;
	newElement->text = NULL;
	newElement->type = LATEX_COMMAND;
	return newElement;
 }

 Element *TextElementSemanticAction(Text *text)
 {
    _logSyntacticAnalyzerAction(__FUNCTION__);
	printf("Entering Text Element: %s\n", text->text);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->text = text;
	newElement->type = LATEX_TEXT;
	return newElement;
 }
