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

/* Estos son los ejemplos de la catedra */
// static const char _expressionTypeToCharacter(const ExpressionType type);
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
// static void _generateEpilogue(const int value);
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generatePrologue(void);


/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
// static void _generatePrologue(void) {
// 	_output(0, "%s",
// 		"\\documentclass{standalone}\n\n"
// 		"\\usepackage[utf8]{inputenc}\n"
// 		"\\usepackage[T1]{fontenc}\n"
// 		"\\usepackage{amsmath}\n"
// 		"\\usepackage{forest}\n"
// 		"\\usepackage{microtype}\n\n"
// 		"\\begin{document}\n"
// 		"    \\centering\n"
// 		"    \\begin{forest}\n"
// 		"        [ \\text{$=$}, circle, draw, purple\n"
// 	);
// }

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
// static void _generateEpilogue(const int value) {
// 	_output(0, "%s%d%s",
// 		"            [ $", value, "$, circle, draw, blue ]\n"
// 		"        ]\n"
// 		"    \\end{forest}\n"
// 		"\\end{document}\n\n"
// 	);
// }
static void _generateEpilogue(const int value) {
	_output(0, "%s",
		"\\end{document}\n\n"
	);
}

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
// static const char _expressionTypeToCharacter(const ExpressionType type) {
// 	switch (type) {
// 		case ADDITION: return '+';
// 		case DIVISION: return '/';
// 		case MULTIPLICATION: return '*';
// 		case SUBTRACTION: return '-';
// 		default:
// 			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
// 			return '\0';
// 	}
// }

/**
 * Generates the output of a constant.
 */
// static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
// 	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
// 	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Generates the output of an expression.
 */
// static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
// 	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
// 	switch (expression->type) {
// 		case ADDITION:
// 		case DIVISION:
// 		case MULTIPLICATION:
// 		case SUBTRACTION:
// 			_generateExpression(1 + indentationLevel, expression->leftExpression);
// 			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, expression->rightExpression);
// 			break;
// 		case FACTOR:
// 			_generateFactor(1 + indentationLevel, expression->factor);
// 			break;
// 		default:
// 			logError(_logger, "The specified expression type is unknown: %d", expression->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

/**
 * Generates the output of a factor.
 */
// static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
// 	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
// 	switch (factor->type) {
// 		case CONSTANT:
// 			_generateConstant(1 + indentationLevel, factor->constant);
// 			break;
// 		case EXPRESSION:
// 			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
// 			_generateExpression(1 + indentationLevel, factor->expression);
// 			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
// 			break;
// 		default:
// 			logError(_logger, "The specified factor type is unknown: %d", factor->type);
// 			break;
// 	}
// 	_output(indentationLevel, "%s", "]\n");
// }

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

    if (content->type == ELEMENT) {
        _generateElement(level, content->element);
    } else if (content->type == SEQUENCE) {
        _generateElement(level, content->sequenceElement);
        _generateContent(level, content->sequenceContent);
    }
}

static void _generateElement(unsigned int level, Element *element) {
    if (!element) return;

    switch (element->type) {
        case LANGTEX_COMMAND:
            _generateLangtexCommand(level, element->langtexCommand);
            break;
        case LATEX_COMMAND:
            _output(level, "%s\n", element->command);
            break;
        case LATEX_TEXT:
            _output(level, "%s\n", element->text->text);
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

static void _generateLangtexCommand(unsigned int level, LangtexCommand *command) {
    if (!command) return;
    switch (command->type) {
        case LANGTEX_TRANSLATE:
			_output(level, "%s\n", command->type);
			// _generateRomanization(level, command->leftContent);
            break;
        case LANGTEX_DIALOG:
		_output(level, "%s\n", command->type);
            break;
        case LANGTEX_EXERCISE:
		_output(level, "%s\n", command->type);
            break;
        case LANGTEX_TABLE:
		_output(level, "%s\n", command->type);
            break;
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
