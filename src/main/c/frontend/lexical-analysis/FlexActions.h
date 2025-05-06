#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
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

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// LaTeX Actions

// TODO
/// @brief Action for the LaTeX comment initialization (`% This is a comment`)
/// @param lexicalAnalyzerContext 
void CommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TODO
/// @brief Action for the LaTeX environment initialization (`\begin{environment}`)
/// @param lexicalAnalyzerContext 
Token BeginCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TODO
/// @brief Action for the LaTeX environment finalization (`\end{function}`)
/// @param lexicalAnalyzerContext 
Token EndCommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TODO
/// @brief Action for command initialization (`\command`)
/// @param lexicalAnalyzerContext 
Token CommandLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TODO
/// @brief Action for brace initialization and finalization (`{}`)
/// @param lexicalAnalyzerContext 
/// @param token 
Token BraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

// TODO
/// @brief Action for basic text in document
/// @param lexicalAnalyzerContext 
Token TextLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
