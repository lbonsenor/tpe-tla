%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char * string;

	/** Non-terminals. */

	Program * program;
	Element * element;
	Content * content;
	Command * command;
	LangtexCommand * langtexCommand;
	Text * text;
}

/**
 * Destructors. These functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't use
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseText($$); } <text>
%destructor { releaseCommand($$); } <command>
%destructor { releaseElement($$); } <element>
%destructor { releaseContent($$); } <content>



/** LaTeX Terminals */
%token <string> COMMAND
%token <token> BEGIN_ENVIRONMENT
%token <token> END_ENVIRONMENT
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <string> TEXT
%token <string> COMMA
%token <string> OPEN_PARENTHESIS
%token <string> CLOSE_PARENTHESIS
%token <string> EQUAL

/** LaNgTeX Terminals */
%token <token> TRANSLATE_COMMAND
%token <token> EXERCISE_COMMAND
%token <token> DIALOG_COMMAND

/** Non-terminals. */

/** LaTeX Non-terminals. */
%type <program> program
%type <content> content
%type <element> element
%type <command> command
%type <text> text

/* LangTeX Non-terminals */
%type <langtexCommand> langtexCommand
/* %type <langtextParameters> langtexParameters
%type <langtextParameter> langtexParameter */

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
// TODO Add LaTeX Non-terminals
program: 
	content 														{ $$ = ContentProgramSemanticAction(currentCompilerState(), $1); }
	;

content:
	element content  												{ $$ = AppendContentSemanticAction($1, $2); }
	| element														{ $$ = SingleContentSemanticAction($1); }
	;

element:
	langtexCommand 			 										{ $$ = LangtexCommandElementSemanticAction($1); }
	| command														{ $$ = CommandElementSemanticAction($1); }
	| text															{ $$ = TextElementSemanticAction($1); }
	;


command:
	BEGIN_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE content END_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE 	
		{ $$ = EnvironmentCommandSemanticAction($3, $5, $8); }
	| COMMAND OPEN_BRACE content CLOSE_BRACE						
		{ $$ = ParameterizedCommandSemanticAction($1, $3); }
	| COMMAND 							 							
		{ $$ = SimpleCommandSemanticAction($1); } 
	;

langtexCommand: 
	TRANSLATE_COMMAND OPEN_BRACE text CLOSE_BRACE OPEN_BRACE text CLOSE_BRACE  { $$ = TranslateSemanticAction($3, $6); }
	;

/* langtexParameters:
	langtexParameter COMMA langtexParameter
	| langtexParameter 
	; */

/* langtexParameter:
	TEXT */

text:
	TEXT 															{ $$ = TextSemanticAction($1); }

%%
