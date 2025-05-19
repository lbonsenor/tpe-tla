#include "FlexActions.h"
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC ACTIONS */

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

/* LaTeX ACTIONS */

Token BeginCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return BEGIN_ENVIRONMENT;
}

Token EndCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext){
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return END_ENVIRONMENT;
}

Token CommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return COMMAND;
}

Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token == OPEN_BRACE;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

void CommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token TextLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
	// printf("%s", lexicalAnalyzerContext->semanticValue->string); -> esto me parece que esta mal, Flex no deberia imprimir nada por consola 
	// “El procesamiento léxico no debe emitir resultados, sino dejar eso al backend.”
	logDebugging(_logger, "TextLexemeAction: %s", lexicalAnalyzerContext->semanticValue->string);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return TEXT;
}

/* LaNgTex ACTIONS */
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    return token;
}

Token LangtexCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return token;
}

 Token EqualsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	lexicalAnalyzerContext->semanticValue->token = EQUAL;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return EQUAL;
 }

 Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = COMMA;
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return COMMA;
 }

 Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}

Token ParameterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	switch(token) {
		case STRING_PARAM:
			return QuotedTextLexemeAction(lexicalAnalyzerContext);
		case INTEGER_PARAM:
			lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
			destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
			return INTEGER_PARAM;
		case BOOLEAN_PARAM:
			lexicalAnalyzerContext->semanticValue->boolean = (strcmp(lexicalAnalyzerContext->lexeme, "true") == 0);
			destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
			return BOOLEAN_PARAM;
		default: 
			lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
			destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
			return ARGS_PARAM;
	}
}

Token QuotedTextLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);

    char * quoted = lexicalAnalyzerContext->lexeme;
    size_t len = strlen(quoted);

    // Ensure length is at least 2 and begins and ends with quotes
    if (len >= 2 && quoted[0] == '"' && quoted[len - 1] == '"') {
        quoted[len - 1] = '\0'; // strip trailing quote
        char * unquoted = strdup(quoted + 1); // skip initial quote
        lexicalAnalyzerContext->semanticValue->string = unquoted;
    } else {
        // fallback, shouldn't happen
        lexicalAnalyzerContext->semanticValue->string = strdup(quoted);
    }
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);

    return STRING_PARAM;
}


