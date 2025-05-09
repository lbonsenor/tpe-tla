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
	Text * text;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
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

/** Non-terminals. */

/** LaTeX Non-terminals. */
%type <program> program
%type <content> content
%type <element> element
%type <command> command
%type <text> text

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
// TODO Add LaTeX Non-terminals
program: content 													{ $$ = ContentProgramSemanticAction(currentCompilerState(), $1); }
	;

content:
	element content  												{ $$ = AppendContentSemanticAction($1, $2); }
	| element														{ $$ = SingleContentSemanticAction($1); }
	;

element:
	command															{ $$ = CommandElementSemanticAction($1); }
	| text															{ $$ = TextElementSemanticAction($1); }
	;

command:
	COMMAND 							 											{ $$ = SimpleCommandSemanticAction($1); }
	| COMMAND OPEN_BRACE content CLOSE_BRACE										{ $$ = ParameterizedCommandSemanticAction($1, $3); }
	| BEGIN_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE content END_ENVIRONMENT OPEN_BRACE text CLOSE_BRACE 	{ $$ = EnvironmentCommandSemanticAction($3, $5, $8); }

text:
	TEXT 															{ $$ = TextSemanticAction($1); }

%%
