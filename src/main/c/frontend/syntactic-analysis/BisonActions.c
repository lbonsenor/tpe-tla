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

 /* Latex semantic actions. */

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

Command * ParameterizedCommandSemanticAction(char * command, ContentList * commandArgs){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	logDebugging(_logger, "Matched parameterized command");
	Command * newCommand = calloc(1, sizeof(Command));
	newCommand->parameterizedCommand = command;
	newCommand->parameterizedContentList = commandArgs;
	newCommand->type = PARAMETERIZED;
	return newCommand;
}

Command * EnvironmentCommandSemanticAction(Text * text, Content * params, ContentList * args, Content * content, Text * text2){
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
	newCommand->environmentParameters = params;
	newCommand->environmentCommandArgs = args;
	newCommand->environmentContent = content;
	newCommand->type = ENVIRONMENT;

	releaseText(text2);
	return newCommand;
}

Text * TextSemanticAction(char * text){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Text * newText = calloc(1, sizeof(Text));
	newText->text = text;
	return newText;
}

Text * NewlineTextSemanticAction(){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Text * newText = calloc(1, sizeof(Text));
	newText->text = malloc(2);
	newText->text[0] = '\n';
	newText->text[1] = '\0';
	return newText;
}

Element * CommandElementSemanticAction(Command *command){
    _logSyntacticAnalyzerAction(__FUNCTION__);
	if (!command) {
		logError(_logger, "CommandElementSemanticAction received NULL command");
		return NULL;
	}
	Element * newElement = calloc(1, sizeof(Element));
	newElement->command = command;
	newElement->type = LATEX_COMMAND;
	return newElement;
}

Element * TextElementSemanticAction(Text * text){
    _logSyntacticAnalyzerAction(__FUNCTION__);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->text = text;
	newElement->type = LATEX_TEXT;
	return newElement;
}

/* Langtex semantic actions. */

Element * LangtexCommandElementSemanticAction(LangtexCommand * langtexCommand){
    _logSyntacticAnalyzerAction(__FUNCTION__);
	Element * newElement = calloc(1, sizeof(Element));
	newElement->langtexCommand = langtexCommand;
	newElement->type = LANGTEX_COMMAND;
	return newElement;
}

LangtexCommand * LangtexSimpleContentSemanticAction(LangtexParamList * parameters, Content * content, LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    LangtexCommand * speakerCommand = calloc(1, sizeof(LangtexCommand));
	speakerCommand->parameters = parameters;
    speakerCommand->content = content;
    speakerCommand->type = type;
    return speakerCommand;
}

LangtexCommand * LangtexCommandListSemanticAction(LangtexParamList * parameters, LangtexCommandList * commandList, LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = parameters;
	langtexCommand->langtexCommandList = commandList;
	langtexCommand->type = type;
	return langtexCommand;
}

LangtexCommand * LangtexContentListSemanticAction(LangtexParamList * parameters, ContentList * contentList, LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
    LangtexCommand * rowCommand = calloc(1, sizeof(LangtexCommand));
	rowCommand->parameters = parameters;
    rowCommand->contentList = contentList;
    rowCommand->type = type;
    return rowCommand;
}

LangtexCommand * TranslateSemanticAction(LangtexParamList *parameters, Content *leftText, Content *rightText) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = parameters;
	langtexCommand->leftText = leftText;
	langtexCommand->rightText = rightText;
	langtexCommand->type = LANGTEX_TRANSLATE;
	return langtexCommand;
}

LangtexCommand * ExerciseSemanticAction(LangtexParamList * parameters, LangtexCommand * commandPrompt, LangtexCommand *commandOptions, LangtexCommand * commandAnswers, LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = parameters;
	langtexCommand->options = commandOptions;
	langtexCommand->answers = commandAnswers;
	langtexCommand->prompt = commandPrompt;

	langtexCommand->type = type;
	return langtexCommand;
}
LangtexCommand * LanguageSemanticAction( TextList * textList, LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = NULL;
	langtexCommand->textList = textList;
	langtexCommand->type = type;
	return langtexCommand;
}

LangtexCommand * FillSemanticAction(LangtexCommandType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	LangtexCommand * langtexCommand = calloc(1, sizeof(LangtexCommand));
	langtexCommand->parameters = NULL;
	langtexCommand->type = type;
	return langtexCommand;
}


/* Langtex Parameter Type Actions */

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

/* Langtex Parameter Actions */

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

/* Utils */

ContentList * ContentListSemanticAction(Content * content, ContentList * next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ContentList * newContentList = calloc(1, sizeof(ContentList));
	newContentList->content = content;
	newContentList->next = next;
	return newContentList;
}

TextList * TextListSemanticAction(Text * text, TextList * next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TextList * newTextList = calloc(1, sizeof(TextList));
	newTextList->text = text;
	newTextList->next = next;
	return newTextList;
}

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
