#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */
static void _generateProgram(Program * program);
static void _generateContent(unsigned int indentationLevel, Content * content);
static void _generateElement(unsigned int indentationLevel, Element * element);
static void _generateLangtexCommand(unsigned int indentationLevel, LangtexCommand * command);
static void _generateProgram(Program * program);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static void _generatePrologue(void);
static void _generateEpilogue(const int value);
static void _generateContentList(unsigned int level, ContentList *contentList);
static void _generateText(unsigned int level, Text *text);
static void _generateParamList(unsigned int level, LangtexParamList * list);
static void _generateParam(unsigned int level, LangtexParam * param);
static void _generateLangtexCommandList(unsigned int level, LangtexCommandList * langtexCommandList);
/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
*/
static void _generatePrologue(void) {
	_output(0, "%s",
		"\\documentclass{article}\n\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage{xeCJK}\n"
		"\\usepackage{array}\n"
		"\\usepackage{amsmath}\n"
		"\\usepackage{xcolor}\n\n"
		"\\begin{document}\n\n"
	);
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */

static void _generateEpilogue(const int value) {
	_output(0, "%s",
		"\\end{document}\n\n"
	);
}



/**
 * Generates the output of the program.
 */
// static void _generateProgram(Program * program) {
// 	_generateExpression(3, program->expression);
// }
static void _generateProgram(Program *program) {
    if (program && program->content) {
        _generateContent(0, program->content);
    }
}

static void _generateContent(unsigned int level, Content *content) {
    if (!content) return;
    _generateElement(level, content->sequenceElement);
    _generateContent(level, content->sequenceContent);
}

static void _generateCommand(unsigned int level, Command *command) {
    if (!command) return;
    switch(command->type){
        case PARAMETERIZED:
        _output(level, "%s", command->parameterizedCommand);
        _output(level,"{");
        _generateContentList(level, command->parameterizedContentList);
        _output(level,"}");
        break;
        case ENVIRONMENT:
        _output(level, "\\begin{%s}", command->environmentLeftText->text);
        _generateContentList(level, command->environmentCommandArgs);
        _output(level, "[");
        _generateContent(level, command->environmentParameters);
        _output(level, "]\n");
        _generateContent(level+1, command->environmentContent);
        _output(level, "\\end{%s}\n", command->environmentLeftText->text); 
        break;
    }
    
}

static void _generateText(unsigned int level, Text *text) {
    if (!text) return;
    _output(level, "%s", text->text);
}

static void _generateContentList(unsigned int level, ContentList *contentList) {
    if (!contentList) return;
    _generateContent(level, contentList->content);
    _generateContentList(level, contentList->next);
}


static void _generateElement(unsigned int level, Element *element) {
    if (!element) return;

    switch (element->type) {
        case LANGTEX_COMMAND:
            _generateLangtexCommand(level, element->langtexCommand);
            break;
        case LATEX_COMMAND:
            _generateCommand(level, element->command);
            break;
        case LATEX_TEXT:
            _output(level, "%s", element->text->text);
            break;
        default:
            logError(_logger, "Unknown element type: %d", element->type);
    }
}



// \text{\color{red}{salud}}
// static void _generateRomanization(unsigned int level, Content * content) {
// 	if (!content || !content->text) return;
// 	Text * text = content->text;
// 	String * outputString = "$$"; 
// 	while(text != NULL) {
		
// 		if (text == ' ' || ) {
// 			// concatenate(level,"%s",outputString);
// 			// concatenate(level,"%s","\\,");
// 			strcat(outputString, "\\,");
// 		}
// 		else {
// 			strcat(outputString, "\\overset{\\text{\\textbf{%s}}}{\\text{\\textbf{%s}}}}", romanizeString(text), text);
// 		}
// 		text = text->next;
// 	}

// 	outputString = strcat(outputString, "$$");
// 	_output(level+1, "%s\n", outputString);
// }
static void _generateParamList(unsigned int level, LangtexParamList * list) {
    if (list == NULL) return;
    _output(level, "(");
    _generateParam(level, list->param);
    _generateParamList(level, list->next);
    _output(level, ")");
}

static void _generateParam(unsigned int level, LangtexParam * param) {
    if (param == NULL) return;
    switch (param->type) {
        case STRING_PARAMETER:
            _output(level, "%s=\"%s\"", param->key, param->value.stringParam);
            break;
        case INTEGER_PARAMETER:
            _output(level, "%s=%d", param->key, param->value.intParam);
            break;
        case BOOLEAN_PARAMETER:
            _output(level, "%s=%s", param->key, param->value.boolParam ? "true" : "false");
            break;
        default:
            logError(_logger, "Unknown parameter type: %d", param->type);
            break;
    }
}


static void _generateLangtexCommandList(unsigned int level, LangtexCommandList * langtexCommandList) {
    if (langtexCommandList == NULL) return;
    _generateLangtexCommand(level, langtexCommandList->command);
    _generateLangtexCommandList(level, langtexCommandList->next);
}
// TODO: change to LATEX format
static void _generateLangtexCommand(unsigned int level, LangtexCommand *command) {
    if (!command) return;
    switch (command->type) {
        case LANGTEX_TRANSLATE:
			_output(level, "[!translate]");
            _generateParamList(level, command->parameters);
            _output(level, "{");
            _generateContent(level, command->leftContent);
            _output(level, "}{");
            _generateContent(level, command->rightContent);
            _output(level, "}\n");
			// _generateRomanization(level, command->leftContent);
            break;
        case LANGTEX_DIALOG:
		    _output(level, "[!dialog]");
            _generateParamList(level, command->parameters);
            _output(level, "{\n");
            _generateLangtexCommandList(level + 1, command->langtexCommandList);
            _output(level, "}\n");
            break;
        case LANGTEX_SPEAKER:
            _output(level, "[!speaker]");
            _generateParamList(level, command->parameters);
            _output(level, "{\n");
            _generateContent(level + 1, command->content);
            _output(level, "}\n");
            break;
        case LANGTEX_EXERCISE:
            _output(level, "[!exercise]");
            _generateParamList(level, command->parameters);
            _output(level, "{\n");
            _generateContent(level + 1, command->leftContent);
            _output(level, "}\n");
            break;
        // case LANGTEX_TABLE:
        //     _output(level, "[!table]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{\n");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}\n");
        //     break;
        // case LANGTEX_ROW:
        //     _output(level, "[!row]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}");
        //     break;
        // case LANGTEX_BLOCK:
        //     _output(level, "[!block]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{\n");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}\n");
        //     break;
        // case LANGTEX_PROMPT:
        //     _output(level, "[!prompt]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{\n");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}\n");
        //     break;
        // case LANGTEX_FILL:
        //     _output(level, "[!fill]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{\n");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}\n");
        //     break;
        // case LANGTEX_ANSWERS:
        //     _output(level, "[!answers]");
        //     _generateParamList(level, command->parameters);
        //     _output(level, "{\n");
        //     _generateContent(level + 1, command->leftContent);
        //     _output(level, "}\n");
        //     break;
        default:
            _output(level, "%% Unsupported command type: %d\\n", command->type);
    }
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating LaNgTeX output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
