#include "Generator.h"

/* MODULE INTERNAL STATE */

static char *_output_buffer = NULL;
static size_t _buffer_size = 0;
static size_t _buffer_pos = 0;
static bool _use_buffer = false;

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;

void initializeGeneratorModule()
{
    _logger = createLogger("Generator");
}

void shutdownGeneratorModule()
{
    if (_logger != NULL)
    {
        destroyLogger(_logger);
    }
}

/** PRIVATE FUNCTIONS */
static void _generateProgram(Program *program);
static void _generateContent(unsigned int indentationLevel, Content *content);
static void _generateElement(unsigned int indentationLevel, Element *element);
static void _generateLangtexCommand(unsigned int indentationLevel, LangtexCommand *command);
static void _generateProgram(Program *program);
static char *_indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char *const format, ...);
static void _generatePrologue(void);
static void _generateEpilogue(const int value);
static void _generateContentList(unsigned int level, ContentList *contentList);
static void _generateText(unsigned int level, Text *text);
static void _generateParamList(unsigned int level, LangtexParamList *list);
static void _generateParam(unsigned int level, LangtexParam *param);
static void _generateLangtexCommandList(unsigned int level, LangtexCommandList *langtexCommandList);
static char *_checkTranslateParam(LangtexParamList *paramList);
static char * _checkSpeakerParam(LangtexParamList *paramList);
static void _generateSpeakerCommand(unsigned int level, LangtexCommand *command);
static void _generateDiaglogCommand(unsigned int level, LangtexCommand *command);
static void _generateExerciseCommand(unsigned int level, LangtexCommand *command);
static void _generateTableCommand(unsigned int level, LangtexCommand *command);
static void _generateRowCommand(unsigned int level, LangtexCommand *command);
static void _generateAnswersCommand(unsigned int level, LangtexCommand *command);
static void _generatePromptCommand(unsigned int level, LangtexCommand *command);
static void _generateBlockCommand(unsigned int level, LangtexCommand *command);
static void _generateExerciseCommand(unsigned int level, LangtexCommand *command);
static void _start_buffering();
static char *_stop_buffering();
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

static void _generatePrologue(void)
{
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
            "\\begin{document}\n\n");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */

static void _generateEpilogue(const int value)
{
    _output(0, "%s",
            "\n\n\\end{document}\n\n");
}

/**
 * Generates the output of the program.
 */
// static void _generateProgram(Program * program) {
// 	_generateExpression(3, program->expression);
// }
static void _generateProgram(Program *program)
{
    if (program && program->content)
    {
        _generateContent(0, program->content);
    }
}

static void _generateContent(unsigned int level, Content *content)
{
    if (!content)
        return;
    _generateElement(level, content->sequenceElement);
    _generateContent(level, content->sequenceContent);
}

static void _generateCommand(unsigned int level, Command *command)
{
    if (!command)
        return;
    switch (command->type)
    {
    case PARAMETERIZED:
        _output(level, "%s", command->parameterizedCommand);
        _output(level, "{");
        _generateContentList(level, command->parameterizedContentList);
        _output(level, "}");
        break;
    case ENVIRONMENT:
        _output(level, "\\begin{%s}", command->environmentLeftText->text);
        _generateContentList(level, command->environmentCommandArgs);
        _output(level, "[");
        _generateContent(level, command->environmentParameters);
        _output(level, "]\n");
        _generateContent(level + 1, command->environmentContent);
        _output(level, "\\end{%s}\n", command->environmentLeftText->text);
        break;
    }
}

static void _generateText(unsigned int level, Text *text)
{
    if (!text)
        return;
    _output(level, "%s", text->text);
}

static void _generateContentList(unsigned int level, ContentList *contentList)
{
    if (!contentList)
        return;
    _generateContent(level, contentList->content);
    _generateContentList(level, contentList->next);
}

static void _generateElement(unsigned int level, Element *element)
{
    if (!element)
        return;

    switch (element->type)
    {
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

static void _generateParamList(unsigned int level, LangtexParamList *list)
{
    if (list == NULL)
        return;
    _output(level, "(");
    _generateParam(level, list->param);
    _generateParamList(level, list->next);
    _output(level, ")");
}

static void _generateParam(unsigned int level, LangtexParam *param)
{
    if (param == NULL)
        return;
    switch (param->type)
    {
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

static void _generateLangtexCommandList(unsigned int level, LangtexCommandList *langtexCommandList)
{
    if (langtexCommandList == NULL)
        return;
    _generateLangtexCommand(level, langtexCommandList->command);
    _generateLangtexCommandList(level, langtexCommandList->next);
}

static char *_checkTranslateParam(LangtexParamList *paramList)
{
    logDebugging(_logger, "Checking for language parameter in translate command.");
    char *language = NULL;
    if (paramList != NULL)
    {
        logDebugging(_logger, "Parameter list is not NULL, checking parameters.");
        while (paramList->param != NULL)
        {
            logDebugging(_logger, "Checking parameter: %s", paramList->param->key);
            if (paramList->param->type == STRING_PARAMETER && strcmp(paramList->param->key, "lang") == 0)
            {
                logDebugging(_logger, "Found language parameter: %s", paramList->param->value.stringParam);
                return paramList->param->value.stringParam;
                break;
            }
            else
            {
                paramList = paramList->next;
            }
        }
    }
    return language;
}

static char * _checkSpeakerParam(LangtexParamList *paramList){

    char *name = "(unknown) Speaker";
    if (paramList != NULL)
    {
        while (paramList->param != NULL)
        {
            if (paramList->param->type == STRING_PARAMETER && strcmp(paramList->param->key, "name") == 0)
            {
                name = paramList->param->value.stringParam;
                break;
            }
            // TODO : are we going to include other parameters? ex. (name="john", age=12, hi=true, hi="hiila")
            else
            {
                paramList = paramList->next;
            }
        }
    }
    return name;
}

static void _generateTranslateCommand(unsigned int level, LangtexCommand *command)
{

    _start_buffering();
    _generateText(level, command->leftText);
    char *left_text = _stop_buffering();

    _start_buffering();
    _generateText(level, command->rightText);
    char *right_text = _stop_buffering();
    logDebugging(_logger, "command paramteres NULL? %s", command->parameters == NULL ? "true" : "false");
    char *language = _checkTranslateParam(command->parameters);
    if (!language)
    {
        logError(_logger, "No language parameter found in translate command.");
        free(left_text);
        free(right_text);
        return;
    }

    char *romanizedWord = romanize(language, left_text);

    _output(level, "\\rom[%s]{%s}{%s}\n",
            right_text,     // hola
            left_text,      // caracteres especiales 안녕하세요
            romanizedWord); // romanizacion
    
    free(romanizedWord);
    free(left_text);
    free(right_text);
}

static void _generateSpeakerCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
        return;
    
    _output(level, "\\textbf{%s:}", _checkSpeakerParam(command->parameters));
    _generateContent(level, command->content);
    _output(level, "\n");
}

static void _generateDiaglogCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
        return;
    
    _output(level, "\n\\begin{tabbing}\n");
// TODO: which params could dialog have?
//     _generateParamList(level, command->parameters);
    _generateLangtexCommandList(level + 1, command->langtexCommandList);
    _output(level, "\\end{tabbing}\n");
}

static void _generateExerciseCommand(unsigned int level, LangtexCommand *command){

}

static void _generateTableCommand(unsigned int level, LangtexCommand *command){

}

static void _generateRowCommand(unsigned int level, LangtexCommand *command){

}

static void _generateBlockCommand(unsigned int level, LangtexCommand *command){

}

static void _generatePromptCommand(unsigned int level, LangtexCommand *command){

}

static void _generateAnswersCommand(unsigned int level, LangtexCommand *command){

}

// TODO: change to LATEX format
static void _generateLangtexCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
        return;
    switch (command->type)
    {
    case LANGTEX_TRANSLATE:
        _generateTranslateCommand(level, command);
        break;
    case LANGTEX_DIALOG:
        _generateDiaglogCommand(level, command);
        break;
    case LANGTEX_SPEAKER:
        _generateSpeakerCommand(level, command);
        break;
    case LANGTEX_EXERCISE:
        _generateExerciseCommand(level, command);
        break;
        // _output(level, "[!exercise]");
        // _generateParamList(level, command->parameters);
        // _output(level, "{\n");
        // _generateContent(level + 1, command->leftContent);
        // _output(level, "}\n");

    case LANGTEX_TABLE:
        _generateTableCommand(level, command);
        break;
    //     _output(level, "[!hebrew_table]");
    //     _generateParamList(level, command->parameters);
    //     _output(level, "{\n");
    //     _generateContent(level + 1, command->leftContent);
    //     _output(level, "}\n");
    case LANGTEX_ROW:
        _generateRowCommand(level, command);
        break;
    //     _output(level, "[!row]");
    //     _generateParamList(level, command->parameters);
    //     _output(level, "{");
    //     _generateContent(level + 1, command->leftContent);
    //     _output(level, "}");

    case LANGTEX_BLOCK:
        _generateBlockCommand(level, command);
        break;
    //     _output(level, "[!block]");
    //     _generateParamList(level, command->parameters);
    //     _output(level, "{\n");
    //     _generateContent(level + 1, command->leftContent);
    //     _output(level, "}\n");
    //     break;
    case LANGTEX_PROMPT:
        _generatePromptCommand(level, command);
        break;
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
    case LANGTEX_ANSWERS:
        _generateAnswersCommand(level, command);
        break;
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
static char *_indentation(const unsigned int level)
{
    return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
// static void _output(const unsigned int indentationLevel, const char * const format, ...) {
// 	va_list arguments;
// 	va_start(arguments, format);
// 	char * indentation = _indentation(indentationLevel);
// 	char * effectiveFormat = concatenate(2, indentation, format);
// 	vfprintf(stdout, effectiveFormat, arguments);
// 	fflush(stdout);
// 	free(effectiveFormat);
// 	free(indentation);
// 	va_end(arguments);
// }

/**  */

// Funciones para manejar el buffer
static void _start_buffering()
{
    _buffer_size = 2048;
    _output_buffer = malloc(_buffer_size);
    _buffer_pos = 0;
    _use_buffer = true;
    if (_output_buffer)
    {
        _output_buffer[0] = '\0';
    }
}

static char *_stop_buffering()
{
    _use_buffer = false;
    char *result = _output_buffer;
    _output_buffer = NULL;
    _buffer_size = 0;
    _buffer_pos = 0;
    return result;
}

// Version modificada de _output que admite modo buffer
static void _output(const unsigned int indentationLevel, const char *const format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    if (_use_buffer && _output_buffer)
    {
        // Modo buffer: escribir al string
        char *indentation = _indentation(indentationLevel);
        char *effectiveFormat = concatenate(2, indentation, format);

        // Calcular el tamaño necesario
        va_list args_copy;
        va_copy(args_copy, arguments);
        int needed_chars = vsnprintf(NULL, 0, effectiveFormat, args_copy);
        va_end(args_copy);

        // Redimensionar buffer si es necesario
        size_t needed_size = _buffer_pos + needed_chars + 1;
        if (needed_size > _buffer_size)
        {
            _buffer_size = needed_size * 2;
            char *new_buffer = realloc(_output_buffer, _buffer_size);
            if (new_buffer)
            {
                _output_buffer = new_buffer;
            }
        }

        // Escribir al buffer
        if (_output_buffer)
        {
            vsprintf(_output_buffer + _buffer_pos, effectiveFormat, arguments);
            _buffer_pos += needed_chars;
        }

        free(effectiveFormat);
        free(indentation);
    }
    else
    {
        // Modo normal: el codigo og del profe
        char *indentation = _indentation(indentationLevel);
        char *effectiveFormat = concatenate(2, indentation, format);
        vfprintf(stdout, effectiveFormat, arguments);
        fflush(stdout);
        free(effectiveFormat);
        free(indentation);
    }

    va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState *compilerState)
{
    logDebugging(_logger, "Generating LaNgTeX output...");
    _generatePrologue();
    _generateProgram(compilerState->abstractSyntaxtTree);
    _generateEpilogue(compilerState->value);
    logDebugging(_logger, "Generation is done.");
}
