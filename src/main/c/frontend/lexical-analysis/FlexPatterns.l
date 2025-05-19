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

%x LANGTEX
%x PARAM

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */



%%
"[!translate]"                      { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), TRANSLATE_COMMAND); }
"[!dialog]"                         { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), DIALOG_COMMAND); }
"[!exercise]"                       { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), EXERCISE_COMMAND); }
"[!speaker]"                        { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), SPEAKER_COMMAND); }
"[!table]"                          { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), TABLE_COMMAND); }
"[!row]"                            { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), ROW_COMMAND); }

<PARAM>,[[:space:]]*                { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<PARAM>"="                          { return EqualsLexemeAction(createLexicalAnalyzerContext()); }           
<PARAM>[[:digit:]]+					{ return ParameterLexemeAction(createLexicalAnalyzerContext(), INTEGER_PARAM); }
<PARAM>\"[^\"]*\"                   { return ParameterLexemeAction(createLexicalAnalyzerContext(), STRING_PARAM); }
<PARAM>true|false					{ return ParameterLexemeAction(createLexicalAnalyzerContext(), BOOLEAN_PARAM); }
<PARAM>[a-zA-Z_-]+                  { return ParameterLexemeAction(createLexicalAnalyzerContext(), ARGS_PARAM); }
<LANGTEX>\(                         { BEGIN(PARAM); return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
<PARAM>\)                           { BEGIN(INITIAL); return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
<PARAM>.                            { return UnknownLexemeAction(createLexicalAnalyzerContext()); } 

"\\begin"                           { return BeginCommandLexemeAction(createLexicalAnalyzerContext()); }
"\\end"                             { return EndCommandLexemeAction(createLexicalAnalyzerContext()); }
\\[a-zA-Z@]+                        { return CommandLexemeAction(createLexicalAnalyzerContext()); }

<LANGTEX>\{                         { BEGIN(INITIAL); return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\{                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\}                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}

[[:space:]]+                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
[^\\{}#\$%\^&_~\[\]]+               { return TextLexemeAction(createLexicalAnalyzerContext()); }

.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%


/* ([[:alnum:]]|[[:punct:]]|[[:alpha:]])+ { return TextLexemeAction(createLexicalAnalyzerContext()); } */
/* \\\\                                { return EscapedTextLexemeAction(createLexicalAnalyzerContext()); }
\\#                                 { return EscapedTextLexemeAction(createLexicalAnalyzerContext()); } */
/* [a-zA-Z0-9@!$#_-]+                  { return TextLexemeAction(createLexicalAnalyzerContext()); } */

#include "FlexExport.h"
