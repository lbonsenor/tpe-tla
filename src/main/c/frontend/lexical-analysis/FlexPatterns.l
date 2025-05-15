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

\{                                  { printf("This is a startbrace\n"); return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\}                                  { printf("This is a brace\n"); return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

\\[a-zA-Z@]+                        { return CommandLexemeAction(createLexicalAnalyzerContext()); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
[[:alnum:][:punct:]]+               { return TextLexemeAction(createLexicalAnalyzerContext()); }

%%

/* LaNgTeX Patterns */

#include "FlexExport.h"
