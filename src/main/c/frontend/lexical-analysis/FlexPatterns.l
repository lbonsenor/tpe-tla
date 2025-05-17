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
"[!translate]"                      { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),TRANSLATE_COMMAND); }
"[!dialog]"                         { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),DIALOG_COMMAND); }
"[!exercise]"                       { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),EXERCISE_COMMAND); }
"[!speaker]"                        { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),SPEAKER_COMMAND); }
"("                                 { return OPEN_PARENTHESIS; }
")"                                 { return CLOSE_PARENTHESIS;}
","                                 { return CommaLexemeAction(createLexicalAnalyzerContext()); }
"="                                 { return EqualsLexemeAction(createLexicalAnalyzerContext()); }

"\\begin"                           { return BeginCommandLexemeAction(createLexicalAnalyzerContext()); }
"\\end"                             { return EndCommandLexemeAction(createLexicalAnalyzerContext()); }

\\[a-zA-Z@]+                        { return CommandLexemeAction(createLexicalAnalyzerContext()); }

\{                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\}                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}
[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

[^\\{}#\$%\^&_~]+                { return TextLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
