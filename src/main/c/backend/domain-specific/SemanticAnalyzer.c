#include "SemanticAnalyzer.h"

static Logger *_logger = NULL;

boolean initializeSemanticAnalyzer(CompilerState *state)
{
    _logger = createLogger("SemanticAnalyzer");
    logDebugging(_logger, "Initializing semantic analyzer for [!translate]");

    // Make sure we have a symbol table
    if (!state->globalSymbolTable)
    {
        state->globalSymbolTable = createSymbolTable();
    }
    return true;
}

void shutdownSemanticAnalyzer(void)
{
    if (_logger)
    {
        destroyLogger(_logger);
        _logger = NULL;
    }
}

/* SUPPORTED LANGUAGES */
static const char *SUPPORTED_LANGUAGES[] = {
    "ko", // Korean
    "zh", // Chinese
    "ja", // Japanese
    "es", // Spanish
    "en", // English
    "fr", // French
    "de", // German
    "it", // Italian
    "ru", // Russian
    "ar", // Arabic
    "he", // Hebrew
    NULL  // Sentinel
};

/* ALLOWED LATEX COMMANDS INSIDE \text{} for TRANSLATE command */
static const char *ALLOWED_LATEX_COMMANDS[] = {
    "\\textbf",    // Bold text
    "\\textit",    // Italic text
    "\\color",     // Color command
    "\\red",       // Red color
    "\\blue",      // Blue color
    "\\green",     // Green color
    "\\black",     // Black color
    "\\underline", // Underline
    "\\emph",      // Emphasis
    NULL           // Sentinel
};

/* HELPER FUNCTIONS FOR TRANSLATE COMMAND */

static boolean isLanguageSupported(const char *langCode)
{
    if (!langCode)
        return false;

    for (int i = 0; SUPPORTED_LANGUAGES[i] != NULL; i++)
    {
        if (strcmp(SUPPORTED_LANGUAGES[i], langCode) == 0)
        {
            return true;
        }
    }
    return false;
}

static boolean validateLatexCommand(char *command)
{
    boolean isAllowed = false;
    for (int i = 0; ALLOWED_LATEX_COMMANDS[i] != NULL; i++)
    {
        if (strcmp(ALLOWED_LATEX_COMMANDS[i], command) == 0)
        {
            logDebugging(_logger, "Allowed LaTeX command: %s", command);
            isAllowed = true;
            break;
        }
    }
    return isAllowed;
}

static boolean validateLatexInCommand(Command *command)
{
    if (!command)
        return true;
    if (command->type != PARAMETERIZED)
    {
        logError(_logger, "Expected parameterized command in [!translate], got: %d", command->type);
        return false;
    }
    validateLatexCommand(command->parameterizedCommand);
    if (command->parameterizedContentList != NULL)
    {
        ContentList *current = command->parameterizedContentList;
        while (current != NULL)
        {
            if (!validateLatexInContent(command->parameterizedContentList->content))
            {
                logError(_logger, "Invalid LaTeX content in command: %s", command->parameterizedCommand);
                return false;
            }
            current = current->next;
        }
    }
}

static boolean validateLatexInContent(Content *content)
{
    if (!content)
        return true; // can be empty
    switch (content->type)
    {
    case ELEMENT:
        return validateLatexInElement(content->sequenceElement);
        break;
    case SEQUENCE:
        // Recursively validate each element in the sequence
        if (!validateLatexInElement(content->sequenceElement))
        {
            return false;
        }
        return validateLatexInContent(content->sequenceContent);
        break;
    default:
        logError(_logger, "Unexpected content type: %d", content->type);
        return false; // Invalid content type
    }
}

static boolean validateLatexInElement(Element *element)
{
    if (!element)
        return true;
    // If it's a text element, we can check it directly
    switch (element->type)
    {
    case LATEX_TEXT:
        return true;
        break;
    case LANGTEX_COMMAND:
        return false;
        break;
    case LATEX_COMMAND:
        return validateLatexInCommand(element->command);
        break;
    default:
        logError(_logger, "Unexpected element type in element of type: %d", element->type);
        return false;
        break;
    }
    return true; // Other element types are not our concern here
}

/* PUBLIC FUNCTIONS */

SemanticAnalysisStatus analyzeProgram(CompilerState *state, Program *program)
{
    if (!program || !program->content)
    {
        logError(_logger, "Empty program");
        return SEMANTIC_ANALYSIS_REJECT;
    }

    logDebugging(_logger, "Starting semantic analysis");
    return analyzeContent(state, program->content);
}

SemanticAnalysisStatus analyzeContent(CompilerState *state, Content *content)
{
    if (!content)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    if (content->type == SEQUENCE)
    {
        // Analyze current element
        SemanticAnalysisStatus status = analyzeElement(state, content->sequenceElement);
        if (status != SEMANTIC_ANALYSIS_ACCEPT)
        {
            return status;
        }

        // Analyze rest of content
        return analyzeContent(state, content->sequenceContent);
    }
    else
    {
        return analyzeElement(state, content->sequenceElement);
    }

    logError(_logger, "Unexpected content type: %d", content->type);
    return SEMANTIC_ANALYSIS_ERROR;
}

SemanticAnalysisStatus analyzeElement(CompilerState *state, Element *element)
{
    if (!element)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    switch (element->type)
    {
    case LANGTEX_COMMAND:
        return analyzeLangtexCommand(state, element->langtexCommand);

    case LATEX_COMMAND:
    case LATEX_TEXT:
        // For now, we accept all LaTeX commands and text
        return SEMANTIC_ANALYSIS_ACCEPT;

    default:
        logError(_logger, "Unknown element type: %d", element->type);
        return SEMANTIC_ANALYSIS_ERROR;
    }
}

SemanticAnalysisStatus analyzeLangtexCommand(CompilerState *state, LangtexCommand *command)
{
    if (!command)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    switch (command->type)
    {
    case LANGTEX_TRANSLATE:
        return analyzeTranslateCommand(state, command);
        break;
    case LANGTEX_TABLE:
        return analyzeTableCommand(state, command);
        break;
    default:
        logError(_logger, "Unknown LangTeX command type: %d", command->type);
        return SEMANTIC_ANALYSIS_ERROR;
    }

    // Accept all other commands for now
    return SEMANTIC_ANALYSIS_ACCEPT;
}

/**
 * [!translate] command
 */
SemanticAnalysisStatus analyzeTranslateCommand(CompilerState *state, LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!translate] command");

    if (!validateLatexInContent(command->leftText))
    {
        logError(_logger, "[!translate] source text contains invalid LaTeX commands");
        logError(_logger, "Only these LaTeX commands are allowed: \\textbf, \\textit, \\color, \\red, \\blue, \\green, \\black, \\underline, \\emph");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    if (!validateLatexInContent(command->rightText))
    {
        logError(_logger, "[!translate] target text contains invalid LaTeX commands");
        logError(_logger, "Only these LaTeX commands are allowed: \\textbf, \\textit, \\color, \\red, \\blue, \\green, \\black, \\underline, \\emph");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    char *languageCode = NULL;
    if (command->parameters)
    {
        LangtexParam *langParam = getParameter(command->parameters, "lang");
        if (langParam)
        {
            if (langParam->type != STRING_PARAMETER)
            {
                logError(_logger, "[!translate] 'lang' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }

            if (!isLanguageSupported(langParam->value.stringParam))
            {
                logError(_logger, "[!translate] unsupported language code: '%s'", langParam->value.stringParam);
                return SEMANTIC_ANALYSIS_ERROR;
            }

            languageCode = langParam->value.stringParam;
            logDebugging(_logger, "[!translate] using language: %s", languageCode);
        }

        // Check for any other parameters (shouldnt exist)
        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "lang") != 0)
            {
                logWarning(_logger, "[!translate] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    if (state->globalSymbolTable)
    {
        // Add language to symbol table if it's not already there
        if (languageCode && !isLanguageDeclared(state->globalSymbolTable, languageCode))
        {

            addLanguage(state->globalSymbolTable, languageCode);
            logDebugging(_logger, "Added language '%s' to symbol table", languageCode);
        }

        logDebugging(_logger, "[!translate] command validation passed");
        return SEMANTIC_ANALYSIS_ACCEPT;
    }
}
/**
 * Simple helper to get parameter by name
 */
LangtexParam *getParameter(LangtexParamList *params, const char *name)
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

SemanticAnalysisStatus analyzeTableCommand(CompilerState *state, LangtexCommand *command)
{
    // For now, we accept all table commands
    logDebugging(_logger, "Analyzing [!table] command");

    // honestly this isnt needed because it only enters this function when it knows that the command aint empty and it is of type langtex table duh
    if (!command || command->type != LANGTEX_TABLE)
    {
        logError(_logger, "Expected [!table] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    int cols = 0;
    if (command->parameters)
    {
        LangtexParam *langParam = getParameter(command->parameters, "cols");
        if (langParam)
        {
            if (langParam->type != INTEGER_PARAMETER)
            {
                logError(_logger, "[!table] 'cols' parameter must be an integer");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            if (langParam->value.intParam <= 0)
            {
                logError(_logger, "[!table] 'cols' parameter must be a positive integer");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!table] using cols: %d", langParam->value.intParam);
            cols = langParam->value.intParam;
        }

        // Check for any other parameters (shouldnt exist)
        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "cols") != 0)
            {
                logWarning(_logger, "[!table] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    LangtexCommandList *current = command->langtexCommandList;
    boolean isFirstRow = true;

    while (current != NULL)
    {
        LangtexCommand *langtexCommand = current->command;

        if (langtexCommand->type != LANGTEX_ROW)
        {
            logError(_logger, "[!table] expected [!row] command, got: %d", langtexCommand->type);
            return SEMANTIC_ANALYSIS_ERROR;
        }

        // Only check "header" param for first row
        if (!isFirstRow && langtexCommand->parameters)
        {
            LangtexParam *headerParam = getParameter(langtexCommand->parameters, "header");
            if (headerParam)
            {
                logError(_logger, "[!row] 'header' parameter is only allowed on the first row");
                return SEMANTIC_ANALYSIS_ERROR;
            }
        }

        if (analyzeRowCommand(state, langtexCommand, cols) != SEMANTIC_ANALYSIS_ACCEPT)
        {
            logError(_logger, "[!table] error analyzing [!row] command");
            return SEMANTIC_ANALYSIS_ERROR;
        }

        isFirstRow = false; // after first row
        current = current->next;
    }

    if (state->globalSymbolTable)
    {
    }

    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeRowCommand(CompilerState *state, LangtexCommand *command, int expectedCols)
{
    logDebugging(_logger, "Analyzing [!row] command");

    if (!command || command->type != LANGTEX_ROW)
    {
        logError(_logger, "Expected [!row] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    if (!command->contentList || !command->contentList->content)
    {
        logError(_logger, "[!row] command must have at least one content");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    if (command->parameters)
    {
        LangtexParam *headerParam = getParameter(command->parameters, "header");
        if (headerParam)
        {
            if (headerParam->type != BOOLEAN_PARAMETER)
            {
                logError(_logger, "[!row] 'header' parameter must be boolean");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!row] using header: %s", headerParam->value.boolParam ? "true" : "false");
        }

        // Check for any other parameters (shouldnt exist)
        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "header") != 0)
            {
                logWarning(_logger, "[!row] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    int cols = 0;
    ContentList *current = command->contentList;
    while (current)
    {
        cols++;
        current = current->next;
    }

    if (cols != expectedCols)
    {
        logError(_logger, "[!row] expected %d columns, got: %d", expectedCols, cols);
        return SEMANTIC_ANALYSIS_ERROR;
    }
    logDebugging(_logger, "[!row] command accepted: has %d columns", cols);

    // For now we accept all row commands
    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeDialogCommand(CompilerState *state, LangtexCommand *command) {
     // For now, we accept all table commands
    logDebugging(_logger, "Analyzing [!dialog] command");

    // honestly this isnt needed because it only enters this function when it knows that the command aint empty and it is of type langtex table duh
    if (!command || command->type != LANGTEX_DIALOG)
    {
        logError(_logger, "Expected [!dialog] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    int cols = 0;
    if (command->parameters)
    {
        LangtexParam *langParam = getParameter(command->parameters, "cols");
        if (langParam)
        {
            if (langParam->type != INTEGER_PARAMETER)
            {
                logError(_logger, "[!table] 'cols' parameter must be an integer");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            if (langParam->value.intParam <= 0)
            {
                logError(_logger, "[!table] 'cols' parameter must be a positive integer");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!table] using cols: %d", langParam->value.intParam);
            cols = langParam->value.intParam;
        }

        // Check for any other parameters (shouldnt exist)
        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "cols") != 0)
            {
                logWarning(_logger, "[!table] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    LangtexCommandList *current = command->langtexCommandList;
    boolean isFirstRow = true;

    while (current != NULL)
    {
        LangtexCommand *langtexCommand = current->command;

        if (langtexCommand->type != LANGTEX_ROW)
        {
            logError(_logger, "[!table] expected [!row] command, got: %d", langtexCommand->type);
            return SEMANTIC_ANALYSIS_ERROR;
        }

        // Only check "header" param for first row
        if (!isFirstRow && langtexCommand->parameters)
        {
            LangtexParam *headerParam = getParameter(langtexCommand->parameters, "header");
            if (headerParam)
            {
                logError(_logger, "[!row] 'header' parameter is only allowed on the first row");
                return SEMANTIC_ANALYSIS_ERROR;
            }
        }

        if (analyzeRowCommand(state, langtexCommand, cols) != SEMANTIC_ANALYSIS_ACCEPT)
        {
            logError(_logger, "[!table] error analyzing [!row] command");
            return SEMANTIC_ANALYSIS_ERROR;
        }

        isFirstRow = false; // after first row
        current = current->next;
    }

    if (state->globalSymbolTable)
    {
    }

    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeSpeakerCommand(CompilerState *state, LangtexCommand *command) {

}


/* STUB FUNCTIONS - Not needed for translate, but required by interface */
SemanticAnalysisStatus analyzeLatexCommand(CompilerState *state, Command *command)
{
    return SEMANTIC_ANALYSIS_ACCEPT;
}

boolean validateParameters(CompilerState *state, LangtexParamList *params, LangtexCommandType expectedFor)
{
    return true;
}

boolean validateParameter(CompilerState *state, LangtexParam *param, const char *expectedName, LangtexParamType expectedType)
{
    return true;
}

boolean validateContext(CompilerState *state, LangtexCommandType commandType)
{
    return true;
}

boolean checkNestingRules(CompilerState *state, LangtexCommandType commandType)
{
    return true;
}