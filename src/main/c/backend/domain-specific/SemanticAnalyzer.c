#include "SemanticAnalyzer.h"

static Logger *_logger = NULL;

void initializeSemanticAnalyzer()
{
    _logger = createLogger("SemanticAnalyzer");
    logDebugging(_logger, "Initializing semantic analyzer for [!translate]");
}

void shutdownSemanticAnalyzer()
{
    if (_logger)
    {
        destroyLogger(_logger);
        _logger = NULL;
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

/* SUPPORTED LANGUAGES */
static const char *SUPPORTED_LANGUAGES[] = {
    "ko", // Korean
    "ru", // Russian
    "he", // Hebrew
    NULL  // Sentinel
};

/* ALLOWED LATEX COMMANDS INSIDE \text{} for TRANSLATE command */
static const char *ALLOWED_LATEX_COMMANDS[] = {
    "\\textbf",    // Bold text
    "\\textit",    // Italic text
    "\\color",     // Color command
    "\\underline", // Underline
    "\\emph",      // Emphasis
    NULL           // Sentinel
};

/* HELPER FUNCTIONS FOR GENERAL COMMAND */
static boolean validateLangtexContent(Content *content)
{
    if (!content)
        return true;

    if (content->type == SEQUENCE)
    {
        if (!validateLangtexElement(content->sequenceElement))
        {
            return false;
        }
        return validateLangtexContent(content->sequenceContent);
    }

    logError(_logger, "[!speaker] unexpected content type: %d", content->type);
    return false;
}

static boolean validateLangtexElement(Element *el)
{
    if (!el)
        return true;

    switch (el->type)
    {
    case LATEX_TEXT:
        return true;
    case LATEX_COMMAND:
        return validateLatexInCommand(el->command);
    case LANGTEX_COMMAND:
        if (el->langtexCommand->type != LANGTEX_TRANSLATE)
        {
            return false;
        }
        return analyzeTranslateCommand(el->langtexCommand) == SEMANTIC_ANALYSIS_ACCEPT;
    default:
        logError(_logger, "[!speaker] invalid element type: %d", el->type);
        return false;
    }
}

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
        return true;
    switch (content->type)
    {
    case SEQUENCE:
        if (!validateLatexInElement(content->sequenceElement))
        {
            return false;
        }
        return validateLatexInContent(content->sequenceContent);
        break;
    default:
        logError(_logger, "Unexpected content type: %d", content->type);
        return false;
    }
}

static boolean validateLatexInElement(Element *element)
{
    if (!element)
        return true;
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
    return true;
}

/* HELPER FUNCTIONS FOR EXERCISE COMMAND */
static boolean validatePromptContent(Content *content)
{
    if (!content)
        return true;

    if (content->type == SEQUENCE)
    {
        if (!validateLangtexElement(content->sequenceElement))
        {
            if (content->sequenceElement->type == LANGTEX_COMMAND && content->sequenceElement->langtexCommand->type == LANGTEX_FILL)
            {
                logDebugging(_logger, "[!fill] found in prompt content");
            }
            else
            {
                return false;
            }
        }
        return validatePromptContent(content->sequenceContent);
    }
    else 
    {
        if (content->sequenceElement->type == LANGTEX_COMMAND && content->sequenceElement->langtexCommand->type == LANGTEX_FILL)
        {
            logDebugging(_logger, "[!fill] found in prompt content");
            return true;
        }
        return validateLangtexElement(content->sequenceElement);
    }

    logError(_logger, "[!speaker] unexpected content type: %d", content->type);
    return false;
}

/* HELPER FUNCTIONS FOR EXERCISE COMMAND */
static boolean validateSingleChoiceExercise(LangtexCommand *prompt, LangtexCommand *answer)
{
    if (prompt->type != LANGTEX_PROMPT || answer->type != LANGTEX_ANSWERS)
    {
        logError(_logger, "[!exercise] single-choice exercise must have a prompt and answer");
        return false;
    }
    if (prompt->content)
    {
        if (!validateLangtexContent(prompt->content))
        {
            logError(_logger, "[!exercise] invalid LaTeX content in prompt");
            return false;
        }
    }

    // validar que la answer sea unica!!
    if (answer->contentList)
    {
        ContentList *current = answer->contentList;
        if (!validateLangtexContent(current->content))
        {
            logError(_logger, "[!exercise] invalid LaTeX content in options");
            return false;
        }
        if (current->next != NULL)
        {
            logError(_logger, "[!exercise] can only have one answer");
            return false;
        }
        return true;
    }
    logError(_logger, "[!exercise] at least one answer is required");
    return false;
}

static boolean validateMultipleChoiceExercise(LangtexCommand *prompt, LangtexCommand *options, LangtexCommand *answers)
{
    if (prompt->type != LANGTEX_PROMPT || options->type != LANGTEX_OPTIONS || answers->type != LANGTEX_ANSWERS)
    {
        logError(_logger, "[!exercise] expected prompt, options, and answers commands");
        return false;
    }
    if (prompt->content)
    {
        if (!validatePromptContent(prompt->content))
        {
            logError(_logger, "[!exercise] invalid LaTeX content in prompt");
            return false;
        }
    }

    int size = 0;
    if (options->contentList)
    {
        ContentList *current = options->contentList;
        while (current)
        {
            if (!validateLangtexContent(current->content))
            {
                logError(_logger, "[!exercise] invalid LaTeX content in options");
                return false;
            }
            size++;
            current = current->next;
        }
    }
    if (size < 1)
    {
        logError(_logger, "[!exercise] at least one option is required");
        return false;
    }

    if (answers->contentList)
    {
        ContentList *current = answers->contentList;
        while (current)
        {
            if (current->content->sequenceContent != NULL && current->content->sequenceElement->type != LATEX_TEXT)
            {
                logError(_logger, "[!exercise] answers must be plain text");
                return false;
            }
            // check if the text is an integer (its gotta be one of the options of the size)
            // atoi(char* text). If there are no digits at all, the result is 0
            logDebugging(_logger, "[!exercise] checking answer: %s", current->content->sequenceElement->text->text);
            int digit = atoi(current->content->sequenceElement->text->text);
            if (digit == 0)
            {
                logError(_logger, "[!exercise] answer is not a digit: '%s'", current->content->sequenceElement->text->text);
                return false;
            }
            if (digit < 0 || digit > size)
            {
                logError(_logger, "[!exercise] answer '%d' is out of range (0-%d)", digit, size);
                return false;
            }
            current = current->next;
        }

        return true; // All answers are valid
    }

    logError(_logger, "[!exercise] at least one answer is required");
    return false;
}

/* PUBLIC FUNCTIONS */

SemanticAnalysisStatus analyzeProgram(Program *program)
{
    logDebugging(_logger, "Starting semantic analysis");
    return analyzeContent(program->content);
}

SemanticAnalysisStatus analyzeContent(Content *content)
{
    if (!content)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    if (content->type == SEQUENCE)
    {
        SemanticAnalysisStatus status = analyzeElement(content->sequenceElement);
        if (status != SEMANTIC_ANALYSIS_ACCEPT)
        {
            return status;
        }

        return analyzeContent(content->sequenceContent);
    }
    else
    {
        return analyzeElement(content->sequenceElement);
    }

    logError(_logger, "Unexpected content type: %d", content->type);
    return SEMANTIC_ANALYSIS_ERROR;
}

SemanticAnalysisStatus analyzeElement(Element *element)
{
    if (!element)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    switch (element->type)
    {
    case LANGTEX_COMMAND:
        return analyzeLangtexCommand(element->langtexCommand);

        // all LATEX commands are accepted -> they will be checked by latex
    case LATEX_COMMAND:
    case LATEX_TEXT:
        return SEMANTIC_ANALYSIS_ACCEPT;
    default:
        logError(_logger, "Unknown element type: %d", element->type);
        return SEMANTIC_ANALYSIS_ERROR;
    }
    logError(_logger, "Unexpected element type: %d", element->type);
    return SEMANTIC_ANALYSIS_ERROR;
}

SemanticAnalysisStatus analyzeLangtexCommand(LangtexCommand *command)
{
    if (!command)
    {
        return SEMANTIC_ANALYSIS_ACCEPT;
    }

    switch (command->type)
    {
    case LANGTEX_TRANSLATE:
        return analyzeTranslateCommand(command);
        break;
    case LANGTEX_TABLE:
        return analyzeTableCommand(command);
        break;
    case LANGTEX_DIALOG:
        return analyzeDialogCommand(command);
        break;
    case LANGTEX_BLOCK:
        return analyzeBlockCommand(command);
        break;
    case LANGTEX_EXERCISE:
        return analyzeExerciseCommand(command);
        break;
    default:
        logError(_logger, "Unknown LangTeX command type: %d", command->type);
        return SEMANTIC_ANALYSIS_ERROR;
    }

    logError(_logger, "Unexpected command type: %d", command->type);
    return SEMANTIC_ANALYSIS_REJECT;
}

SemanticAnalysisStatus analyzeTranslateCommand(LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!translate] command");

    if (!validateLatexInContent(command->leftText))
    {
        logError(_logger, "[!translate] source text contains invalid LaTeX commands");
        logError(_logger, "Only these LaTeX commands are allowed: \\textbf, \\textit, \\color, \\underline, \\emph");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    if (!validateLatexInContent(command->rightText))
    {
        logError(_logger, "[!translate] target text contains invalid LaTeX commands");
        logError(_logger, "Only these LaTeX commands are allowed: \\textbf, \\textit, \\color, \\underline, \\emph");
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
    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeTableCommand(LangtexCommand *command)
{
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

        if (analyzeRowCommand(langtexCommand, cols) != SEMANTIC_ANALYSIS_ACCEPT)
        {
            logError(_logger, "[!table] error analyzing [!row] command");
            return SEMANTIC_ANALYSIS_ERROR;
        }

        isFirstRow = false; // after first row
        current = current->next;
    }

    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeRowCommand(LangtexCommand *command, int expectedCols)
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

SemanticAnalysisStatus analyzeDialogCommand(LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!dialog] command");

    // honestly this isnt needed because it only enters this function when it knows that the command aint empty and it is of type langtex table duh
    if (!command || command->type != LANGTEX_DIALOG)
    {
        logError(_logger, "Expected [!dialog] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    if (command->parameters)
    {
        LangtexParam *titleParam = getParameter(command->parameters, "title");
        if (titleParam)
        {
            if (titleParam->type != STRING_PARAMETER)
            {
                logError(_logger, "[!dialog] 'title' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!dialog] using title: %s", titleParam->value.stringParam);
        }

        LangtexParam *styleParam = getParameter(command->parameters, "style");
        if (styleParam)
        {
            if (styleParam->type != INTEGER_PARAMETER)
            {
                logError(_logger, "[!dialog] 'style' parameter must be an integer");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            if (styleParam->value.intParam < 0 || styleParam->value.intParam > 2)
            {
                logError(_logger, "[!dialog] 'style' parameter must be between 0 and 2");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!dialog] using style: %d", styleParam->value.intParam);
        }

        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "title") != 0 && strcmp(current->param->key, "style") != 0)
            {
                logWarning(_logger, "[!dialog] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    LangtexCommandList *current = command->langtexCommandList;
    while (current != NULL)
    {
        LangtexCommand *langtexCommand = current->command;

        if (langtexCommand->type != LANGTEX_SPEAKER)
        {
            logError(_logger, "[!dialog] expected [!speaker] command, got: %d", langtexCommand->type);
            return SEMANTIC_ANALYSIS_ERROR;
        }

        SemanticAnalysisStatus status = analyzeSpeakerCommand(langtexCommand);
        if (status != SEMANTIC_ANALYSIS_ACCEPT)
        {
            logError(_logger, "[!dialog] error analyzing [!speaker] or [!block] command");
            return SEMANTIC_ANALYSIS_ERROR;
        }

        current = current->next;
    }

    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeSpeakerCommand(LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!speaker] command");

    if (!command || command->type != LANGTEX_SPEAKER)
    {
        logError(_logger, "Expected [!speaker] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    if (!command->content)
    {
        logError(_logger, "[!speaker] command must have content");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    if (command->parameters)
    {
        LangtexParam *nameParam = getParameter(command->parameters, "name");
        if (nameParam)
        {
            if (nameParam->type != STRING_PARAMETER)
            {
                logError(_logger, "[!speaker] 'name' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!speaker] using name: %s", nameParam->value.stringParam);
        }
        // Check for any other parameters (shouldnt exist)
        LangtexParamList *current = command->parameters;
        while (current && current->param)
        {
            if (strcmp(current->param->key, "name") != 0)
            {
                logWarning(_logger, "[!speaker] unknown parameter '%s'", current->param->key);
            }
            current = current->next;
        }
    }

    if (!validateLangtexContent(command->content))
    {
        logError(_logger, "[!speaker] only [!translate] allowed inside [!speaker]");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    // For now we accept all speaker commands
    return SEMANTIC_ANALYSIS_ACCEPT;
}

// using duplicate validation and warning
SemanticAnalysisStatus analyzeBlockCommand(LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!block] command");

    if (!command || command->type != LANGTEX_BLOCK)
    {
        logError(_logger, "Expected [!block] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    if (!command->content)
    {
        logError(_logger, "[!block] command must have content");
        return SEMANTIC_ANALYSIS_ERROR;
    }

    LangtexParamList *current = command->parameters;
    while (current && current->param)
    {
        const char *key = current->param->key;
        LangtexParam *param = current->param;

        // Check for duplicates
        LangtexParam *first = getParameter(command->parameters, key);
        if (first != param)
        {
            logWarning(_logger, "[!block] duplicate parameter '%s' found — only the first will be used", key);
            current = current->next;
            continue; // Skip further processing for duplicates
        }

        // Handle known parameter
        if (strcmp(key, "title") == 0)
        {
            if (param->type != STRING_PARAMETER)
            {
                logError(_logger, "[!block] 'title' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!block] using title: %s", param->value.stringParam);
        }
        else
        {
            logWarning(_logger, "[!block] unknown parameter '%s'", key);
        }

        current = current->next;
    }

    if (!validateLangtexContent(command->content))
    {
        return SEMANTIC_ANALYSIS_ERROR;
    }

    return SEMANTIC_ANALYSIS_ACCEPT;
}

SemanticAnalysisStatus analyzeExerciseCommand(LangtexCommand *command)
{
    logDebugging(_logger, "Analyzing [!exercise] command");

    if (!command || command->type != LANGTEX_EXERCISE)
    {
        logError(_logger, "Expected [!exercise] command, got: %d", command ? command->type : -1);
        return SEMANTIC_ANALYSIS_REJECT;
    }

    LangtexParamList *current = command->parameters;
    char *typeParam = NULL;
    while (current && current->param)
    {
        const char *key = current->param->key;
        LangtexParam *param = current->param;

        // Check for duplicates
        LangtexParam *first = getParameter(command->parameters, key);
        if (first != param)
        {
            logWarning(_logger, "[!exercise] duplicate parameter '%s' found — only the first will be used", key);
            current = current->next;
            continue; // Skip further processing for duplicates
        }

        // Handle known parameter
        if (strcmp(key, "type") == 0)
        {
            if (param->type != STRING_PARAMETER)
            {
                logError(_logger, "[!exercise] 'type' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            typeParam = param->value.stringParam;
            logDebugging(_logger, "[!exercise] using type: %s", param->value.stringParam);
        }
        else if (strcmp(key, "title") == 0)
        {
            if (param->type != STRING_PARAMETER)
            {
                logError(_logger, "[!exercise] 'title' parameter must be a string");
                return SEMANTIC_ANALYSIS_ERROR;
            }
            logDebugging(_logger, "[!exercise] using title: %s", param->value.stringParam);
        }
        else
        {
            logWarning(_logger, "[!exercise] unknown parameter '%s'", key);
        }

        current = current->next;
    }

    if (strcmp(typeParam, "multiple-choice") == 0)
    {
        if (command->prompt == NULL || command->answers == NULL || command->options == NULL)
        {
            logError(_logger, "[!exercise] [!fill] command must have prompt, answers, and options");
            return SEMANTIC_ANALYSIS_ERROR;
        }
        if (validateMultipleChoiceExercise(command->prompt, command->options, command->answers))
        {
            logDebugging(_logger, "[!exercise] [!multiple-choice] command validation passed");
            return SEMANTIC_ANALYSIS_ACCEPT;
        }
        else
        {
            logError(_logger, "[!exercise] [!multiple-choice] command validation failed");
            return SEMANTIC_ANALYSIS_ERROR;
        }
    }
    else if (strcmp(typeParam, "single-choice") == 0)
    {
        if (command->prompt == NULL || command->answers == NULL)
        {
            logError(_logger, "[!exercise] [!fill] command must have prompt, answers");
            return SEMANTIC_ANALYSIS_ERROR;
        }
        if (command->options != NULL)
        {
            logWarning(_logger, "[!exercise] [!single-choice] command should not have options");
        }
        if (validateSingleChoiceExercise(command->prompt, command->answers))
        {
            logDebugging(_logger, "[!exercise] [!single-choice] command validation passed");
            return SEMANTIC_ANALYSIS_ACCEPT;
        }
        else
        {
            logError(_logger, "[!exercise] [!single-choice] command validation failed");
            return SEMANTIC_ANALYSIS_ERROR;
        }
        logError(_logger, "[!exercise] unsupported exercise type: %s", typeParam);
        return SEMANTIC_ANALYSIS_ERROR;
    }
}