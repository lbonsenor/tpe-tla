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

%%

"\\begin"                           { return BeginCommandLexemeAction(createLexicalAnalyzerContext()); }
"\\end"                             { return EndCommandLexemeAction(createLexicalAnalyzerContext()); }

\{                                  { printf("This is a startbrace\n"); return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\}                                  { printf("This is a brace\n"); return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

\\[a-zA-Z@]+                        { return CommandLexemeAction(createLexicalAnalyzerContext()); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
[^\\{}#\$%\^&_~]+                   { printf("This is a text\n"); return TextLexemeAction(createLexicalAnalyzerContext()); }

%%

/* LaNgTeX Patterns */

#include "FlexExport.h"
