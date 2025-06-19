#include "Langtex.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeLangtexModule() {
	_logger = createLogger("Langtex");
}

void shutdownLangtexModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static ComputationResult _invalidComputation();
/**
 * A computation that always returns an invalid result.
 */
static ComputationResult _invalidComputation() {
	ComputationResult computationResult = {
		.succeed = false,
		.value = 0
	};
	return computationResult;
}

/** PUBLIC FUNCTIONS */

ComputationResult computeContent(Content * content){
	logDebugging(_logger, "In compute content function");
	if (content == NULL) {
		logError(_logger, "Content is NULL");
		return (ComputationResult){
					.succeed = true,
					.value = 0 // Placeholder value, actual logic needed
				};
	}
	logDebugging(_logger, "Content type is SEQUENCE?: %s",content->type == SEQUENCE ? "YES" : "NO");


	switch(content->type){
		// case ELEMENT: 
		// 	return computeElement(content->element);
		case SEQUENCE: 
			ComputationResult sequenceElementResult = computeElement(content->sequenceElement);
			ComputationResult sequenceContentResult;
	
			if (content->sequenceContent != NULL) {
				sequenceContentResult = computeContent(content->sequenceContent);
			}
			else {
				sequenceContentResult = (ComputationResult){
					.succeed = true,
					.value = 0 // Placeholder value, actual logic needed
				};
			}

			
			if (sequenceElementResult.succeed && sequenceContentResult.succeed){
					return (ComputationResult){
							.succeed = true,
							.value = 0 // Placeholder value, actual logic needed
						};
				// todo something with langtex...? 
				// BinaryOperator binaryOperator = _expressionTypeToBinaryOperator(expression->type);
				// return binaryOperator(leftResult.value, rightResult.value);
			}
			else {
				return _invalidComputation();
			}
			break;
		default:
			logError(_logger, "Unknown content type: %d", content->type);
			return _invalidComputation();
	}
}

ComputationResult computeElement(Element * element) {
	switch(element->type) {
		case LANGTEX_COMMAND: return computeLangtexCommand(element->langtexCommand);
		case LATEX_COMMAND: return computeCommand(element->command);
		case LATEX_TEXT: return computeText(element->text);
		default:
			logError(_logger, "Unknown element type: %d", element->type);
			return _invalidComputation();
	}
}

ComputationResult computeText(Text * text){
	return (ComputationResult){
		.succeed = true,
		.value = 0 // Placeholder value, actual logic needed
	};
	//todo return something
}

// case LANGTEX_EXERCISE:
// 			ComputationResult promptResult = computeLangtexCommand(langtexCommand->prompt);
// 			ComputationResult optionsResult = computeLangtexCommand(langtexCommand->options);
// 			ComputationResult answersResult = computeLangtexCommand(langtexCommand->answers);
// 			if (parametersResult.succeed && answersResult.succeed && optionsResult.succeed && promptResult.succeed) {
// 				// Handle the exercise logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute exercise components.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		case LANGTEX_DIALOG:
// 		case LANGTEX_TABLE:
// 			ComputationResult commandListResult = computeLangtexCommandList(langtexCommand->langtexCommandList);
// 			if (parametersResult.succeed && commandListResult.succeed) {
// 				// Handle dialog or table logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute command list in dialog or table command.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		case LANGTEX_SPEAKER:
// 		case LANGTEX_PROMPT:
// 		case LANGTEX_BLOCK:
// 			ComputationResult contentResult = computeContent(langtexCommand->content);
// 			if (parametersResult.succeed && contentResult.succeed) {
// 				// Handle speaker, prompt, or block logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute content in speaker, prompt, or block command.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		case LANGTEX_ROW:
// 		case LANGTEX_OPTIONS:
// 		case LANGTEX_ANSWERS:
// 			ComputationResult contentListResult = computeContentList(langtexCommand->contentList);
// 			if (parametersResult.succeed && contentListResult.succeed) {
// 				// Handle row, options, or answers logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute content list in row, options, or answers command.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		case LANGTEX_LANGUAGE:
// 			ComputationResult textListResult = computeTextList(langtexCommand->textList);
// 			if (parametersResult.succeed && textListResult.succeed) {
// 				// Handle language command logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute text list in language command.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		case LANGTEX_FILL:
// 			ComputationResult textResult = computeText(langtexCommand->text);
// 			if (parametersResult.succeed && textResult.succeed) {
// 				// Handle fill command logic, e.g., storing or printing the result.
// 				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
// 			} else {
// 				logError(_logger, "Failed to compute text in fill command.");
// 				return _invalidComputation();
// 			}
// 			break;
// 		default:
// 			logError(_logger, "Unknown langtex command type: %d", langtexCommand->type);
// 			return _invalidComputation();
// 	}

// TODO: all of these functions need an actual computation success implementation.
ComputationResult computeLangtexCommand(LangtexCommand * langtexCommand) {
	ComputationResult parametersResult = computeParamList(langtexCommand->parameters);
	switch (langtexCommand->type) {
		case LANGTEX_TRANSLATE:
			ComputationResult leftContentResult = computeContent(langtexCommand->leftContent);
			ComputationResult rightContentResult = computeContent(langtexCommand->rightContent);
			if (parametersResult.succeed && leftContentResult.succeed && rightContentResult.succeed) {
				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
			} else {
				logError(_logger, "Failed to compute left or right content in translation command.");
				return _invalidComputation();
			}
			break;
		case LANGTEX_DIALOG:
		case LANGTEX_TABLE:
			ComputationResult commandListResult = computeLangtexCommandList(langtexCommand->langtexCommandList);
			if (parametersResult.succeed && commandListResult.succeed) {
				// Handle dialog or table logic, e.g., storing or printing the result.
				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
			} else {
				logError(_logger, "Failed to compute command list in dialog or table command.");
				return _invalidComputation();
			}
			break;
		case LANGTEX_ROW:
		case LANGTEX_OPTIONS:
		case LANGTEX_ANSWERS:
			ComputationResult contentListResult = computeContentList(langtexCommand->contentList);
			if (parametersResult.succeed && contentListResult.succeed) {
				return (ComputationResult){.succeed = true, .value = 0};
			} else {
				logError(_logger, "Failed to compute content list in row, options, or answers command.");
				return _invalidComputation();
			}
			break;
		case LANGTEX_SPEAKER:
		case LANGTEX_PROMPT:
		case LANGTEX_BLOCK:
			ComputationResult contentResult = computeContent(langtexCommand->content);
			if (parametersResult.succeed && contentResult.succeed) {
				return (ComputationResult){.succeed = true, .value = 0}; // Placeholder value
			} else {
				logError(_logger, "Failed to compute content in speaker, prompt, or block command.");
				return _invalidComputation();
			}
			break;
	}
		return _invalidComputation(); 
}

ComputationResult computeLangtexCommandList(LangtexCommandList * langtexCommandList) {
	if (langtexCommandList != NULL) {
		ComputationResult commandResult = computeLangtexCommand(langtexCommandList->command);
		if (commandResult.succeed) {
			ComputationResult nextCommandResult = computeLangtexCommandList(langtexCommandList->next);
			if (nextCommandResult.succeed) {
				return (ComputationResult){
					.succeed = true,
					.value = 0
				}; 
			} else {
				return _invalidComputation();
			}
		}
	}
	return (ComputationResult){
					.succeed = true,
					.value = 0 
				}; 
}



ComputationResult computeCommand(Command * command) {
	switch (command->type){
		// case SIMPLE: return (ComputationResult){.succeed = true, .value = 0}; //todo hacer algo con simpleCommand
		case PARAMETERIZED:  
		//todo hacer algo con parameterizedCommand
			logDebugging(_logger, "In compute COMMAND function, is the list null? %s", command->parameterizedContentList == NULL ? "YES" : "NO");
			ComputationResult result = computeContentList(command->parameterizedContentList);
			break;
		case ENVIRONMENT:
			ComputationResult textResult = computeText(command->environmentLeftText);
			ComputationResult parametersResult = computeContent(command->environmentParameters);
			ComputationResult contentListResult = computeContentList(command->environmentCommandArgs);
			ComputationResult contentResult = computeContent(command->environmentContent);
			if (textResult.succeed && parametersResult.succeed && contentListResult.succeed && contentResult.succeed) {
				return (ComputationResult){
					//todo add logic here?
					.succeed = true, 
					.value = 0
				}; 
			} else {
				logError(_logger, "Failed to compute environment command.");
				return _invalidComputation();
			}
	}
}

//todo: is this list well handled like this?
ComputationResult computeTextList(TextList * textList){
	if (textList!=NULL){
		ComputationResult textResult = computeText(textList->text);
		if (textResult.succeed) {
			ComputationResult nextTextResult = computeTextList(textList->next);
			if (nextTextResult.succeed) {
				return (ComputationResult){
					.succeed = true, 
					.value = 0 // Placeholder value, actual logic needed
					// .value = something
				}; 
			} else {
				return _invalidComputation();
			}
		} else {
			return _invalidComputation();
		}
	}
}

//todo: is this list well handled like this?
ComputationResult computeContentList(ContentList * contentList){
	logDebugging(_logger, "In compute CONTENT LIST, is the list null? %s", contentList == NULL ? "YES" : "NO");
	
	if (contentList != NULL) {
		ComputationResult contentResult = computeContent(contentList->content);
		if (contentResult.succeed) {
			ComputationResult nextContentResult = computeContentList(contentList->next);
			if (nextContentResult.succeed) {
				return (ComputationResult){
					.succeed = true,
					.value = 0 
					// .value = something
				}; 
			} else {
				return _invalidComputation();
			}
		}
	}
	return (ComputationResult){
					.succeed = true,
					.value = 0 
					// .value = something
				}; 
}

ComputationResult computeParam(LangtexParam* param){
	//something?
	return (ComputationResult) {
		.succeed = true,
		.value = 0 // Placeholder value, actual logic needed
	};
}

ComputationResult computeParamList(LangtexParamList * paramList) {
	if (paramList != NULL) {
		ComputationResult paramResult = computeParam(paramList->param);
		if (paramResult.succeed) {
			ComputationResult nextParamResult = computeParamList(paramList->next);
			if (nextParamResult.succeed) {
				return (ComputationResult){
					.succeed = true,
					.value = 0 
					// .value = something
				}; 
			} else {
				return _invalidComputation();
			}
		}
	}
	return (ComputationResult){
					.succeed = true,
					.value = 0 
					// .value = something
				};
}