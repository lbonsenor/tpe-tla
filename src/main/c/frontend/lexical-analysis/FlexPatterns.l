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
%x LANGTEX_EXPRESSION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

/* 
[!t]{
    }
*/
/* <LANGTEX_EXPRESSION>"([a-zA-Z]+)"   { return BeginLangtexParameterAction(createLexicalAnalyzerContext()); } */
/* \[![a-zA-Z]+\]                      { return LangTexLexemeAction(createLexicalAnalyzerContext()); } */

/* 

[!translate         BeginLangtexcommand....

LANGTEX_COMMAND CLOSE_BRACKET                                                                           simple
LANGTEX_COMMAND OPEN_PARENTESIS text CLOSE_PARENTESIS CLOSE_BRACKET                                     parameterized
LANGTEX_COMMAND CLOSE_BRACKET OPEN_BRACE content CLOSE_BRACE                                            environment
LANGTEX_COMMAND OPEN_PARENTESIS text CLOSE_PARENTESIS CLOSE_BRACKET OPEN_BRACE content CLOSE_BRACE      parameterized_environment

<LANGTEX_EXPRESSION>"\""([^\"\\]|\\.)*\"[[:space:]]*,[[:space:]]*\""([^\"\\]|\\.)*\"" 
                                    { return TranslationContentLexemeAction();}

 */

%%
"[!translate]"                      { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),TRANSLATE_COMMAND); }
"[!dialog]"                         { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),DIALOG_COMMAND); }
"[!exercise]"                       { return LangtexCommandLexemeAction(createLexicalAnalyzerContext(),EXERCISE_COMMAND); }    
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

/** LaNgTeX Patterns */

#include "FlexExport.h"
