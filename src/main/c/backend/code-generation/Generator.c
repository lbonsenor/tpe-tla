#include "Generator.h"

/* MODULE INTERNAL STATE */

static char *_output_buffer = NULL;
static size_t _buffer_size = 0;
static size_t _buffer_pos = 0;
static bool _use_buffer = false;

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger *_logger = NULL;
static LangtexParam *getParameter(LangtexParamList *params, const char *name);

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
static void _generateEpilogue();
static void _generateContentList(unsigned int level, ContentList *contentList);
static void _generateText(unsigned int level, Text *text);
static void _generateParamList(unsigned int level, LangtexParamList *list);
static void _generateParam(unsigned int level, LangtexParam *param);
static void _generateLangtexCommandList(unsigned int level, LangtexCommandList *langtexCommandList);
static char *_checkTranslateParam(LangtexParamList *paramList);
static char *_checkSpeakerParam(LangtexParamList *paramList);
static void _generateSpeakerCommand(unsigned int level, LangtexCommand *command);
static void _generateDialogCommand(unsigned int level, LangtexCommand *command);
static void _generateExerciseCommand(unsigned int level, LangtexCommand *command);
static void _generateTableCommand(unsigned int level, LangtexCommand *command);
static void _generateRowCommand(unsigned int level, LangtexCommand *command);
static void _generateAnswersCommand(unsigned int level, LangtexCommand *answersCommand, ContentList *optionsContentList);
static void _generateOptionsCommand(unsigned int level,  LangtexCommand *command);
static void _generatePromptCommand(unsigned int level, LangtexCommand *command);
static void _generateBlockCommand(unsigned int level, LangtexCommand *command);
static void _generateExerciseCommand(unsigned int level, LangtexCommand *command);
static void _generateFillCommand(unsigned int level, LangtexCommand *command);
static void _start_buffering();
static char *_stop_buffering();

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 */
static void _generatePrologue() {
	_output(0, "%s\n%s\n%s\n",
        "\\documentclass[12pt, a4paper, openany]{book}",
		"\\input{preamble.tex}",
        "\\begin{document}"
	);
}


/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */

static void _generateEpilogue()
{
    _output(0, "%s",
            "\n\\end{document}");
}

/**
 * Generates the output of the program.
 */
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
        _generateContent(level, command->environmentContent);
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
    char *language = NULL;
    if (paramList != NULL)
    {
        while (paramList->param != NULL)
        {
            if (paramList->param->type == STRING_PARAMETER && strcmp(paramList->param->key, "lang") == 0)
            {
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

static char *_checkSpeakerParam(LangtexParamList *paramList)
{

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
    _generateContent(level, command->leftText);
    char *left_content = _stop_buffering();

    // _start_buffering();
    // _generateContent(level, command->rightText);
    // char *right_content = _stop_buffering();
    char *language = _checkTranslateParam(command->parameters);
    if (!language)
    {
        logError(_logger, "No language parameter found in translate command.");
        return;
    }

    char *romanizedWord = romanize(language, left_content);
    if (romanizedWord == NULL)
    {
        logError(_logger, "Failed to romanize the word: %s", left_content);
        free(left_content);
        // free(right_content);
        return;
    }

    _output(level, "\\rom[");
    _generateContent(level, command->rightText); // right content
    _output(level, "]{%s}{%s}", left_content, romanizedWord);
    
           
    // _output(level, "\\rom[%s]{%s}{%s}",
    //         right_content,  // translation
    //         left_content,   // caracteres especiales 안녕하세요
    //         romanizedWord!=NULL? romanizedWord : "this is null!! " ); // romanizacion

    free(romanizedWord);
    free(left_content);
    // free(right_content);
}

static void _generateSpeakerCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
        return;

    LangtexParam * speakerName = getParameter(command->parameters, "name");
    _output(level, "\n\t\\speaker[%s]", speakerName ? speakerName->value.stringParam : "");
    _output(level, "{");
    _generateContent(level, command->content);
    _output(level, "}");
}

static void _generateDialogCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
        return;

    _output(level, "\n\\begin{dialog}");
    _generateLangtexCommandList(level, command->langtexCommandList);
    _output(level, "\n\\end{dialog}");
}
 
static void _generateTableCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
    {
        return;
    }

    int cols = 0;
    if (command->parameters)
    {
        LangtexParam *colsParam = getParameter(command->parameters, "cols");
        if (colsParam)
        {
            cols = colsParam->value.intParam;
        }
    }

    LangtexCommandList *current = command->langtexCommandList;

    // in latex: |c|c|c|c|
    size_t maxlen = 1 + 2 * cols + 1;
    char *columCs = malloc(maxlen);
    columCs[0] = '|';
    columCs[1] = '\0';
    for (int i = 0; i < cols; i++)
    {
        strcat(columCs, "c|");
    }

    _output(level, "\\begin{tabular}{%s}\n", columCs);
    free(columCs);
    while (current != NULL)
    {
        LangtexCommand *rowCommand = current->command; // [!row]

        _generateRowCommand(level + 1, rowCommand);
        current = current->next;
    }
    _output(level, "\\end{tabular}\n");
}

static void _generateRowCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
    {
        return;
    }
    ContentList *current = command->contentList;
    _output(level, "");
    LangtexParam *param = getParameter(command->parameters, "header");

    bool isHeader = param != NULL && param->value.boolParam;

    while (current != NULL)
    {
        if (isHeader)
            _output(0, "\\textbf{");
        _generateContent(0, current->content);
        if (isHeader)
            _output(0, "}");
        current = current->next;
        if (current != NULL)
            _output(0, " & ");
        else
            _output(level, "\n");
    }
}

static void _generateBlockCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
    {
        return;
    }
    _output(level, "\\begin{tcolorbox}[box={%s}]\n", command->parameters ? command->parameters->param->value.stringParam : "Block");
    _generateContent(level, command->content);
    _output(level, "\\end{tcolorbox}");
}


// IF Prompt: ToggleExercise
// ELSE: Exercise
static void _generateExerciseCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
    {
        return;
    }
    LangtexParam *exerciseType = getParameter(command->parameters, "type");
    LangtexParam *exerciseTitle = getParameter(command->parameters, "title");
    _output(level, "\\Exercise\n\t{");
    _generateLangtexCommand(level, command->prompt);
    _output(level, "}\n\t");
   
    if (command->options!=NULL){
        _output(level, "[");
        _generateOptionsCommand(level, command->options);
        _output(level, "]\n\t");
    } else {
        _output(level, "[]\n\t");
    }
    _output(level, "[");
    _generateAnswersCommand(level, command->answers, command->options ? command->options->contentList : NULL);
    _output(level, "]\n\t[%s]", exerciseTitle ? exerciseTitle->value.stringParam : "");
}

static void _generatePromptCommand(unsigned int level, LangtexCommand *command)
{
    if (!command)
    {
        return;
    }
    _generateContent(level, command->content);
}

static void _generateOptionsCommand(unsigned int level,  LangtexCommand *command)
{ 
    if (!command)
    {
        return ;
    }
    ContentList* current = command->contentList;
    int count = 0;
    while (current != NULL)
    {
        _generateContent(0, current->content);
        current = current->next;
        if (current != NULL)
            _output(0, ", ");
    }
}

static void _generateAnswersCommand(unsigned int level, LangtexCommand *answersCommand, ContentList *optionsContentList)
{
    if (!answersCommand)
    {
        return;
    }
    if (optionsContentList == NULL)
    {
        ContentList *current = answersCommand->contentList;
        _generateContent(level, current->content);
    } else {
        //multiple answers
        ContentList *currentAnswers = answersCommand->contentList;
        //save the digits in an array
        int size = 10;
        int * answers = malloc(sizeof(int) * size);
        int count = 0;

        while (currentAnswers != NULL) {
            // ya validado que va a ser un digito desde semantic analyzer
            int digit = atoi(currentAnswers->content->sequenceElement->text->text);
            answers[count++] = digit;
            if (count >= size) {
                logDebugging(_logger, "Increasing answers array size from %d to %d", 10, 20);
                answers = realloc(answers, sizeof(int) * (count + size));
            }
            currentAnswers = currentAnswers->next;
        }
        int iterator = 1;
        // {3}{1}{2}
        // 3 ==1 ? no, sigo. 1==1? si, imprimo.
        while(optionsContentList != NULL)
        {
            for (int i = 0; i < count; i++){
                if (answers[i]==iterator){
                    _generateContent(level, optionsContentList->content);
                    if (i < count - 1)
                        _output(level, ",");
                    break;
                }
            }
            iterator++;
            optionsContentList = optionsContentList->next;
        }
        free(answers);
    }
}

static void _generateFillCommand(unsigned int level, LangtexCommand *command)
{
    _output(level, "\\fillLine ");
    // TODO: look at fill later in bison grammar
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
        _generateDialogCommand(level, command);
        break;
    case LANGTEX_SPEAKER:
        _generateSpeakerCommand(level, command);
        break;
    case LANGTEX_EXERCISE:
        _generateExerciseCommand(level, command);
        break;
    case LANGTEX_TABLE:
        _generateTableCommand(level, command);
        break;
    case LANGTEX_ROW:
        _generateRowCommand(level, command);
        break;
    case LANGTEX_BLOCK:
        _generateBlockCommand(level, command);
        break;
    case LANGTEX_PROMPT:
        _generatePromptCommand(level, command);
        break;
    case LANGTEX_FILL:
        _generateFillCommand(level, command);
        break;
    // case LANGTEX_ANSWERS:
    //     _generateAnswersCommand(level, command, char *options[], int arraySize);
    //     break;
    // case LANGTEX_OPTIONS:
    //     _generateptionsCommand(level, command);
    //     break;
    default:
        _output(level, "%% Unsupported command type: %d\\n", command->type);
    }
}

static LangtexParam *getParameter(LangtexParamList *params, const char *name)
{
    LangtexParamList *current = params;
    while (current && current->param)
    {
        if (current->param->key && strcmp(current->param->key, name) == 0)
        {
            return current->param;
        }
        current = current->next;
    }
    return NULL;
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

void generate(char * outputDir, char * fileName, bool isInput, CompilerState *compilerState)
{
    char* outputPath = NULL;
    FILE *original_stdout = NULL;

    if (outputDir != NULL) {
        // CREATE DIRECTORY IF IT DOESN'T EXIST
        struct stat st = {0};
        if (stat(outputDir, &st) == -1) {
            if (mkdir(outputDir, 0755) == -1) {
                logError(_logger, "Could not create directory: %s", outputDir);
                return;
            }
            logDebugging(_logger, "Created directory: %s", outputDir);
        }

        // Create symlink to preamble.tex

        char* preambleLinkPath = malloc(strlen(outputDir) + strlen("/preamble.tex") + 1);
        sprintf(preambleLinkPath, "%s/preamble.tex", outputDir);

        unlink(preambleLinkPath); // si ya existia, lo borra 

        // Crear nuevo symlink (path relativo desde outputDir)
        const char* preambleSource = "../references/preamble.tex";
        if (symlink(preambleSource, preambleLinkPath) == -1) {
            logError(_logger, "Failed to create preamble symlink: %s -> %s", preambleLinkPath, preambleSource);
        } else {
            logDebugging(_logger, "Created preamble symlink: %s -> %s", preambleLinkPath, preambleSource);
        }
        
        free(preambleLinkPath);

        // Make path for output file

        size_t len = strlen(outputDir);
        bool needs_slash = (len > 0 && outputDir[len - 1] != '/');
        outputPath = malloc(strlen(outputDir) + strlen(fileName) + 2);

        if (outputPath == NULL) {
            logError(_logger, "Memory allocation failed for outputPath.");
            return;
        }
        sprintf(outputPath, "%s%s%s", outputDir, needs_slash ? "/" : "", fileName);

        FILE *fd = fopen(outputPath, "w");
        if (fd == NULL) {
            logError(_logger, "Could not create/open file: %s", outputPath);
            free(outputPath);
            return;
        }
        
        logDebugging(_logger, "Generating LaNgTeX output to file: %s", outputPath);
        
        original_stdout = stdout;
        stdout = fd;
    } else {
        logDebugging(_logger, "Generating LaNgTeX output to stdout");
    }

    if (!isInput) _generatePrologue();
    _generateProgram(compilerState->abstractSyntaxtTree);
    if (!isInput) _generateEpilogue();

    if (outputDir != NULL && original_stdout != NULL) {
        FILE *fd = stdout;
        stdout = original_stdout;
        
        if (fclose(fd) != 0) {
            logError(_logger, "Error while closing file");
        }
    }

    logDebugging(_logger, "Generation is done.");
    free(outputPath);

    // if (outputDir != NULL) {
    //     size_t len = strlen(outputDir);
    //     bool needs_slash = (len > 0 && outputDir[len - 1] != '/');
    //     outputPath = malloc(strlen(outputDir) + strlen(fileName) + 2); // +1 for '/' or '\0', +1 for '\0'

    //     if (outputPath == NULL) {
    //         logError(_logger, "Memory allocation failed for outputPath.");
    //         return;
    //     }
    //     sprintf(outputPath, "%s%s%s", outputDir, needs_slash ? "/" : "", fileName);

    //     FILE *fd = fopen(outputPath, "w");
    //     if (fd == NULL) {
    //         logError(_logger, "Could not create/open file: %s", outputPath);
    //         if (!outputPath) free(outputPath);
    //         return;
    //     }
        
    //     logDebugging(_logger, "Generating LaNgTeX output to file: %s", outputPath);
        
    //     // Save original stdout and redirect to file
    //     original_stdout = stdout;
    //     stdout = fd;
    // } else {
    //     logDebugging(_logger, "Generating LaNgTeX output to stdout");
    // }

    // // Generate the content
    // if (!isInput) _generatePrologue();
    // _generateProgram(compilerState->abstractSyntaxtTree);
    // if (!isInput) _generateEpilogue();

    // // Restore stdout if we redirected it
    // if (outputDir != NULL && original_stdout != NULL) {
    //     FILE *fd = stdout;
    //     stdout = original_stdout;
        
    //     if (fclose(fd) != 0) {
    //         logError(_logger, "Error while losing file");
    //     }
        
    // }

    // logDebugging(_logger, "Generation is done.");
    // free(outputPath);

}
