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
// static void _generatePrologue(void) {
// 	_output(0, "%s",
// 		// "\\input{preamble.tex}",
//         "\\begin{document}"
// 	);
// }

static void _generatePrologue(void) {
    _output(0, "%s",
        "% Must be compiled with XeLaTeX\n"
        "\n"
        "% Font & CJK\n"
        "\\usepackage{fontspec}\n"
        "\\newfontfamily\\ipafont{Charis SIL}\n"
        "\\usepackage{xeCJK}\n"
        "\n"
        "\\setCJKsansfont{DotumChe}\n"
        "\\setCJKmainfont[\n"
        "  Path = ./fonts/,\n"
        "  UprightFont = NotoSansKR-Regular.ttf,\n"
        "  BoldFont = NotoSerifKR-Bold.ttf,\n"
        "  SansFont = NotoSerifKR-Regular.ttf\n"
        "]{NotoSansKR}\n"
        "\n"
        "% Math\n"
        "\\usepackage{amsmath,amssymb}\n"
        "\n"
        "% Layout\n"
        "\\usepackage{graphicx}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{titlesec}\n"
        "\\usepackage{multicol}\n"
        "\\geometry{margin=1in}\n"
        "\n"
        "% Hyperlinks and bookmarks\n"
        "\\usepackage{hyperref}\n"
        "\\usepackage{bookmark}\n"
        "\n"
        "% Color boxes\n"
        "\\usepackage[most]{tcolorbox}\n"
        "\n"
        "\\usepackage{tabularx, cellspace}\n"
        "\\usepackage{menukeys}\n"
        "\\usepackage{indentfirst}\n"
        "\\usepackage{glossaries}\n"
        "\\usepackage{tikz}\n"
        "\\usepackage{array}\n"
        "\\def\\checkmark{\\tikz\\fill[scale=0.4](0,.35) -- (.25,0) -- (1,.7) -- (.25,.15) -- cycle;}\n"
        "\n"
        "\\titleformat{\\chapter}[hang]{\\normalfont\\huge\\bfseries}{\\thechapter.}{1em}{}\n"
        "\n"
        "% Custom Korean vocab box\n"
        "\\tcbset{\n"
        "  box/.style={\n"
        "    enhanced,\n"
        "    attach boxed title to top center={yshift=-3mm,yshifttext=-1mm},\n"
        "    title=#1\n"
        "  }\n"
        "}\n"
        "\n"
        "\\newcolumntype{C}{|X|}\n"
        "\n"
        "\\newcommand{\\ipa}[1]{{\\ipafont /#1/}}\n"
        "\n"
        "\\newcommand{\\spacedstack}[1]{\\vspace{0.3ex}\\shortstack{#1}\\vspace{0.3ex}}\n"
        "\n"
        "\\newcommand\\rom[3][]{\n"
        "  \\ifx\\relax#1\\relax\n"
        "    $\\overset{\\text{\\color{red}#3}}{\\text{#2}}$\n"
        "  \\else\n"
        "    $\\underset{\\textbf{#1}}{\\overset{\\text{\\color{red}#3}}{\\text{#2}}}$\n"
        "  \\fi\n"
        "}\n"
        "\n"
        "\\newcommand{\\cross}{$\\times$}\n"
        "\n"
        "\\begin{document}\n"
    );
}


/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */

static void _generateEpilogue(const int value) {
	_output(0, "%s",
		"\n\\end{document}\n\n"
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

static void _generateTranslateCommand(unsigned int level, LangtexCommand * command) {
    char *text = command->leftContent->sequenceElement->text->text;
    if (!text) return;
    
    char* romanizedWord = romanizeHangul(text);
    
    if (romanizedWord) {
        _output(level, "\\rom[]{%s}{%s}{%s}", 
               text, 
               romanizedWord, 
               command->rightContent->sequenceElement->text->text);
        free(romanizedWord);
    }
}

// static void _generateTranslateCommand(unsigned int level, LangtexCommand * command) {
//     setlocale(LC_ALL, "en_US.UTF-8");

//     // char * to wchar_t
//     char *narrow = command->leftContent->sequenceElement->text->text;
//     logError(_logger, "Narrow: %s", narrow);
//     if (!narrow) return;

//     size_t wlen = mbstowcs(NULL, narrow, 0);
//     if (wlen == (size_t)-1) {
//         // Error en la conversión
//         logError(_logger, "Error: Invalid multibyte sequence\n");
//         return;
//     }

//     // wchar_t *wide = malloc((wlen + 1) * sizeof(wchar_t));
//     // mbstowcs(wide, narrow, wlen + 1);

//     // Asignar memoria (+1 para null terminator)
//     wchar_t *wide = malloc((wlen + 1) * sizeof(wchar_t));
//     if (!wide) return;
    
//     // Hacer la conversión real
//     size_t result = mbstowcs(wide, narrow, wlen + 1);
//     if (result == (size_t)-1) {
//         // Error en la conversión
//         printf("Error: Conversion failed\n");
//         free(wide);
//         return;
//     }
    
//     // Asegurar null termination
//     wide[wlen] = L'\0';
    
//     char* romanizedWord = romanizeHangul(wide);

//     if (romanizedWord) {
//         _output(level, "\\rom[]{%s}{%s}{%s}", 
//                command->leftContent->sequenceElement->text->text, 
//                romanizedWord, 
//                command->rightContent->sequenceElement->text->text);
//         free(romanizedWord);
//     }
    
//     free(wide);

    // _output(level, "\\rom[]{%s}{%s}{%s}", command->leftContent->sequenceElement->text->text, romanizedWord, command->rightContent->sequenceElement->text->text);
    // free(wide);
    // free(romanizedWord);
    // despues cambiarlo para que pueda recibir tambien leftContent y rightContent si hay mas de un elemento
    // ademas SequenceElement puede tener un langtex command o un latex command o text.
    // Por ahora solo funciona para [!translate](){text1}{text2}{text3}


    // _output(level, "[!translate]");
    // _generateParamList(level, command->parameters);
    // _output(level, "{");
    // _generateContent(level, command->leftContent);
    // _output(level, "}{");
    // _generateContent(level, command->rightContent);
    // _output(level, "}\n");
    // _generateRomanization(level, command->leftContent);

// }
// TODO: change to LATEX format
static void _generateLangtexCommand(unsigned int level, LangtexCommand *command) {
    if (!command) return;
    switch (command->type) {
        case LANGTEX_TRANSLATE:
            _generateTranslateCommand(level, command);
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
        //     _output(level, "[!hebrew_table]");
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
