#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/LangTeXAST.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// LaTeX Actions

/// @brief Action for the LaTeX comment initialization (`% This is a comment`)
/// @param lexicalAnalyzerContext 
void CommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/// @brief Action for the LaTeX environment initialization (`\begin{environment}`)
/// @param lexicalAnalyzerContext 
Token BeginCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/// @brief Action for the LaTeX environment finalization (`\end{function}`)
/// @param lexicalAnalyzerContext 
Token EndCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/// @brief Action for command initialization (`\command`)
/// @param lexicalAnalyzerContext 
Token CommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/// @brief Action for brace initialization and finalization (`{}`)
/// @param lexicalAnalyzerContext 
/// @param token 
Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

/// @brief Action for basic text in document
/// @param lexicalAnalyzerContext 
Token TextLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

/* LaNgTex ACTIONS */

Token LangtexCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EqualsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token QuotedTextLexemeAction(LexicalAnalyzerContext * context);
Token ParameterLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
#endif
