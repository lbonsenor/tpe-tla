#include "LangTeXAST.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeRenameMeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownRenameMeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseParamList(LangtexParamList * list) {
	if (list != NULL) {
		releaseParam(list->param);
		releaseParamList(list->next);
		free(list);
	}
}

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

void releaseText(Text * text) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (text != NULL) {
		free(text->text);
        free(text);
	}
}

void releaseCommand(Command * command){
    if (command != NULL){
        switch (command->type) {
            case SIMPLE:
                free(command->simpleCommand);
                break;
            case PARAMETERIZED:
                free(command->parameterizedCommand);
                releaseContent(command->parameterizedContent);
                break;
            case ENVIRONMENT:
                releaseText(command->environmentLeftText);
                releaseContent(command->environmentContent);
                releaseText(command->environmentRightText);
                break;
            default:
                break;
        }
        free(command);
    }
}

void releaseLangtexCommand(LangtexCommand * langtexCommand){
    if (langtexCommand != NULL){
        switch (langtexCommand->type) {
            case LANGTEX_TRANSLATE:
                releaseContent(langtexCommand->rightContent);
                releaseContent(langtexCommand->leftContent);
                releaseParamList(langtexCommand->parameters); 
                break;
            case LANGTEX_DIALOG:
                releaseObject(langtexCommand->objectList);
                releaseParamList(langtexCommand->parameters); 
                break;
            default:
                break;
        }
        free(langtexCommand);
    }
}

void releaseObject(Object * object) {
    if (object != NULL){
        // switch (object->type){
        //     case OBJECT_SPEAKER:
                releaseParamList(object->parameters);
                releaseContent(object->content);
                releaseObject(object->next); 
                free(object);
            //     break;
            // default:
            //     break;
        // };
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
