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

 Command * ParameterizedCommandSemanticAction(char * command, ContentList * commandArgs){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	logDebugging(_logger, "Matched parameterized command");
	Command * newCommand = calloc(1, sizeof(Command));
	newCommand->parameterizedCommand = command;
	newCommand->parameterizedContentList = commandArgs;
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

 ContentList * ContentListSemanticAction(Content * content, ContentList * next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ContentList * newContentList = calloc(1, sizeof(ContentList));
	newContentList->content = content;
	newContentList->next = next;
	return newContentList;
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

 LangtexCommand * TranslateSemanticAction(LangtexParamList *parameters, Content *leftContent, Content *rightContent)
 {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = parameters;
	langtexCommand->leftContent = leftContent;
	langtexCommand->rightContent = rightContent;
	langtexCommand->type = LANGTEX_TRANSLATE;
	return langtexCommand;
 }

 LangtexCommand * DialogSemanticAction(LangtexParamList * parameters, LangtexCommandList * langtexCommandList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
    langtexCommand->parameters = parameters;
    langtexCommand->langtexCommandList = langtexCommandList;
    langtexCommand->type = LANGTEX_DIALOG;
    return langtexCommand;
 }

LangtexCommand * SpeakerSemanticAction(LangtexParamList * parameters, Content * content) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    LangtexCommand * speakerCommand = calloc(1, sizeof(LangtexCommand));
	speakerCommand->parameters = parameters;
    speakerCommand->content = content;
    speakerCommand->type = LANGTEX_SPEAKER;
    return speakerCommand;
}

//Table

LangtexCommand * TableSemanticAction(LangtexParamList * parameters, LangtexCommandList * commandList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = parameters;
	langtexCommand->langtexCommandList = commandList;
	langtexCommand->type = LANGTEX_TABLE;
	return langtexCommand;
}

LangtexCommand * RowSemanticAction(LangtexParamList * parameters, ContentList * contentList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    LangtexCommand * rowCommand = calloc(1, sizeof(LangtexCommand));
	rowCommand->parameters = parameters;
    rowCommand->contentList = contentList;
    rowCommand->type = LANGTEX_ROW;
    return rowCommand;
}

// Parameters

LangtexParam * IntegerParamSemanticAction(char * key, int value) {
	LangtexParam * param = calloc(1, sizeof(LangtexParam));
	param->key = key;
	param->value.intParam = value;
	param->type = INTEGER_PARAMETER;
	return param;
}

LangtexParam * StringParamSemanticAction(char * key, char * value){
	LangtexParam * param = calloc(1, sizeof(LangtexParam));
	param->key = key;
	param->value.stringParam = value;
	param->type = STRING_PARAMETER;
	return param;
}

LangtexParam * BooleanParamSemanticAction(char * key, boolean value){
	LangtexParam * param = calloc(1, sizeof(LangtexParam));
	param->key = key;
	param->value.boolParam = value;
	param->type = BOOLEAN_PARAMETER;
	return param;
}

LangtexParamList * SingleParam(LangtexParam * param) {
    LangtexParamList * list = calloc(1, sizeof(LangtexParamList));
    list->param = param;
    list->next = NULL;
    return list;
}
LangtexParamList * AppendParam(LangtexParam * param, LangtexParamList * list) {
    LangtexParamList * newList = calloc(1, sizeof(LangtexParamList));
    newList->param = param;
    newList->next = list;
    return newList;
}

LangtexParamList * EmptyParamList(void) {
    return NULL;
}

// Utils
LangtexCommandList * AppendLangtexComand(LangtexCommand * langtexCommand, LangtexCommandList * langtexCommandList) {
    LangtexCommandList * newList = calloc(1, sizeof(LangtexCommandList));
	newList->command = langtexCommand;
	newList->next = langtexCommandList;
	return newList;
}

LangtexCommandList * SingleLangtexCommand(LangtexCommand * command) {
	LangtexCommandList * list = calloc(1, sizeof(LangtexCommandList));
	list->command = command;
	list->next = NULL;
	return list;
}
