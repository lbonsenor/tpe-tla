%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

/* TODO: \t{ is allowed for some reason, but \t { isn't (as it should) */

%%

"\\begin"                           { return BeginCommandLexemeAction(createLexicalAnalyzerContext()); }
"\\end"                             { return EndCommandLexemeAction(createLexicalAnalyzerContext()); }

"{"                                 { return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

"\\[a-zA-Z@]+"                      { return CommandLexemeAction(createLexicalAnalyzerContext(), COMMAND); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
[[:alnum:][:punct:]]+               { return TextLexemeAction(createLexicalAnalyzerContext()); }

%%

/* LaNgTeX Patterns */

#include "FlexExport.h"
