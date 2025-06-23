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
"[!speaker]"                        { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), SPEAKER_COMMAND); }
"[!table]"                          { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), TABLE_COMMAND); }
"[!row]"                            { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), ROW_COMMAND); }
"[!exercise]"                       { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), EXERCISE_COMMAND); }
"[!prompt]"                         { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), PROMPT_COMMAND); }
"[!options]"                        { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), OPTIONS_COMMAND); }
"[!answer]"                         { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), ANSWER_COMMAND); }
"[!block]"                          { BEGIN(LANGTEX); return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), BLOCK_COMMAND);}
"[!fill]"                           { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(), FILL_COMMAND); }

\\[\\{}#\$%\^&_~\[\]]               { return TextLexemeAction(createLexicalAnalyzerContext()); }

<PARAM>,[[:space:]]*                { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<PARAM>"="                          { return EqualsLexemeAction(createLexicalAnalyzerContext()); }           
<PARAM>[[:digit:]]+					{ return ParameterLexemeAction(createLexicalAnalyzerContext(), INTEGER_PARAM); }
<PARAM>\"[^\"]*\"                   { return ParameterLexemeAction(createLexicalAnalyzerContext(), STRING_PARAM); }
<PARAM>true|false					{ return ParameterLexemeAction(createLexicalAnalyzerContext(), BOOLEAN_PARAM); }
<PARAM>[a-zA-Z_-]+                  { return ParameterLexemeAction(createLexicalAnalyzerContext(), ARGS_PARAM); }
<LANGTEX>\(                         { BEGIN(PARAM); return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
<PARAM>\)                           { BEGIN(LANGTEX); return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
<PARAM>.                            { return UnknownLexemeAction(createLexicalAnalyzerContext()); } 

"\\begin"                           { return BeginCommandLexemeAction(createLexicalAnalyzerContext()); }
"\\end"                             { return EndCommandLexemeAction(createLexicalAnalyzerContext()); }
\\[a-zA-Z@]+                        { return CommandLexemeAction(createLexicalAnalyzerContext()); }

<LANGTEX>\{                         { BEGIN(INITIAL); return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\{                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
\}                                  { return BraceLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }

\[                                  { return BracketLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACKET); }
\]                                  { return BracketLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACKET); }

"%"[^\n]*                           { CommentLexemeAction(createLexicalAnalyzerContext());}

\n                                  { return NEWLINE; }

[ \t\r]+                            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

[^\\{}#\$%\^_~\[\]\n]+              { return TextLexemeAction(createLexicalAnalyzerContext()); }



.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%


#include "FlexExport.h"

