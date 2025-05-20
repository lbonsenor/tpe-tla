#include "LangTeXAST.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeRenameMeModule() {
	_logger = createLogger("LangtexSyntaxTree");
}

void shutdownRenameMeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

/* Langtex Commands */

void releaseLangtexCommand(LangtexCommand * langtexCommand){
    if (langtexCommand != NULL){
        switch (langtexCommand->type) {
            case LANGTEX_TRANSLATE:
                releaseContent(langtexCommand->rightContent);
                releaseContent(langtexCommand->leftContent);
                releaseParamList(langtexCommand->parameters); 
                break;
            case LANGTEX_DIALOG:
            case LANGTEX_TABLE:
                releaseLangtexCommandList(langtexCommand->langtexCommandList);
                releaseParamList(langtexCommand->parameters); 
                break;
            case LANGTEX_SPEAKER:
            case LANGTEX_PROMPT:
            case LANGTEX_BLOCK:
                releaseContent(langtexCommand->content);
                releaseParamList(langtexCommand->parameters); 
                break;
            case LANGTEX_ROW:
            case LANGTEX_OPTIONS:
            case LANGTEX_ANSWERS:
                releaseContentList(langtexCommand->contentList);
                releaseParamList(langtexCommand->parameters);
                break;        
            case LANGTEX_EXERCISE: 
                releaseLangtexCommand(langtexCommand->answers);
                releaseLangtexCommand(langtexCommand->options);
                releaseLangtexCommand(langtexCommand->prompt);
                releaseParamList(langtexCommand->parameters);
                break;
            case LANGTEX_LANGUAGE:
                releaseTextList(langtexCommand->textList);
                releaseParamList(langtexCommand->parameters);
                break;
            case LANGTEX_FILL:
                releaseText(langtexCommand->text);
            default:
                break;
        }
        free(langtexCommand);
    }
}

void releaseLangtexCommandList(LangtexCommandList * langtexCommandList) {
    if (langtexCommandList != NULL){
        releaseLangtexCommand(langtexCommandList->command);
        releaseLangtexCommandList(langtexCommandList->next); 
        free(langtexCommandList);
    }
}

/* Langtex Parameters */

void releaseParam(LangtexParam * param) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (param != NULL) {
        free(param->key);
        if (param->type == STRING_PARAMETER) {
            free(param->value.stringParam);
        } 
        // else if (param->value.stringParam != NULL) {
		// 	free(param->value.stringParam);  
		// }
        free(param);
    }
    
}

void releaseParamList(LangtexParamList * list) {
	if (list != NULL) {
		releaseParam(list->param);
		releaseParamList(list->next);
		free(list);
	}
}

/* Latex commands */
void releaseCommand(Command * command){
    if (command != NULL){
        switch (command->type) {
            case SIMPLE:
                free(command->simpleCommand);
                break;
            case PARAMETERIZED:
                free(command->parameterizedCommand);
                releaseContentList(command->parameterizedContentList);
                break;
            case ENVIRONMENT:
                releaseText(command->environmentLeftText);
                releaseContent(command->environmentParameters);
                releaseContentList(command->environmentCommandArgs);
                releaseContent(command->environmentContent);
                break;
            default:
                break;
        }
        free(command);
    }
}

void releaseContentList(ContentList * contentList) {
    if (contentList != NULL){
        releaseContent(contentList->content);
        releaseContentList(contentList->next);
        free(contentList);
    }
}

void releaseTextList(TextList * textList) {
    if (textList != NULL){
        releaseText(textList->text);
        releaseTextList(textList->next);
        free(textList);
    }
}

void releaseText(Text * text) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (text != NULL) {
		free(text->text);
        free(text);
	}
}

void releaseElement(Element * element){
    if (element != NULL){
        switch (element->type) {
            case LANGTEX_COMMAND:
                releaseLangtexCommand(element->langtexCommand);
                break;
            case LATEX_COMMAND:
                releaseCommand(element->command);
                break;
            case LATEX_TEXT:
                releaseText(element->text);
                break;
            default:
                break;
        }
        free(element);
    }
}

void releaseContent(Content * content){
    if (content != NULL){
        switch (content->type) {
            case ELEMENT:
                releaseElement(content->element);
                break;
            case SEQUENCE:
                releaseElement(content->sequenceElement);
                releaseContent(content->sequenceContent);
                break;
            default:
                break;
        }
        free(content);
    }
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseContent(program->content);
		free(program);
	}
}
