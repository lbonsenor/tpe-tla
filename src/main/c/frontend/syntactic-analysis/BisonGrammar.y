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
	boolean boolean;

	/** Non-terminals. */

	Program * program;
	Element * element;
	Content * content;
	ContentList * contentList;
	Command * command;
	LangtexCommand * langtexCommand;
	LangtexCommandList * langtexCommandList;
	LangtexParam * param;
	LangtexParamList * param_list;
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
%destructor { releaseLangtexCommand($$); } <langtexCommand>
%destructor {releaseLangtexCommandList($$); } <langtexCommandList>
%destructor { releaseElement($$); } <element>
%destructor { releaseContent($$); } <content>
%destructor { releaseParamList($$); } <param_list>
%destructor { releaseParam($$); } <param>


/** LaTeX Terminals */
%token <string> COMMAND
%token <token> BEGIN_ENVIRONMENT
%token <token> END_ENVIRONMENT
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <string> TEXT

/** LaNgTeX Terminals */
%token <token> TRANSLATE_COMMAND
%token <token> EXERCISE_COMMAND
%token <token> DIALOG_COMMAND
%token <token> SPEAKER_COMMAND
%token <token> TABLE_COMMAND
%token <token> ROW_COMMAND

%token <token> COMMA
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> EQUAL
%token <string> ARGS_PARAM

%token <integer> INTEGER_PARAM
%token <string> STRING_PARAM
%token <boolean> BOOLEAN_PARAM

/** Other Terminals */ 
%token <token> UNKNOWN

/** Non-terminals. */

/** LaTeX Non-terminals. */
%type <program> program
%type <content> content
%type <contentList> commandArgs
%type <element> element
%type <command> command
%type <text> text

/** LaNgTeX Non-terminals **/
%type <langtexCommand> langtexCommand speakerCommand rowCommand 
%type <langtexCommandList> speakerCommands
%type <langtexCommandList> rowCommands

%type <param> param
%type <param_list> param_list
%type <param_list> parameters
%type <param_list> optional_parameters

/* %type <langtextParameters> langtexParameters
%type <langtextParameter> langtexParameter */

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
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
	| COMMAND commandArgs
																	{ $$ = ParameterizedCommandSemanticAction($1,$2); }
	;

commandArgs:
	OPEN_BRACE content CLOSE_BRACE commandArgs						{ $$ = ContentListSemanticAction($2, $4); }
	| %empty														{ $$ = NULL; }
	;

langtexCommand:
	TRANSLATE_COMMAND optional_parameters OPEN_BRACE content CLOSE_BRACE OPEN_BRACE content CLOSE_BRACE  
																	{ $$ = TranslateSemanticAction($2, $4, $7); }
	| DIALOG_COMMAND optional_parameters OPEN_BRACE speakerCommands CLOSE_BRACE 
																	{ $$ = DialogSemanticAction($2, $4); }
	| TABLE_COMMAND optional_parameters OPEN_BRACE rowCommands CLOSE_BRACE 
																	{ $$ = TableSemanticAction($2, $4); }
	;

rowCommands:
	rowCommand rowCommands 											{ $$ = AppendLangtexComand($1, $2); }
	| rowCommand 													{ $$ = SingleLangtexCommand($1); } 
	;

rowCommand:
	ROW_COMMAND optional_parameters commandArgs						{ $$ = RowSemanticAction($2, $3); }
	;

speakerCommands:
	speakerCommand speakerCommands									{ $$ = AppendLangtexComand($1, $2); }
	| speakerCommand 												{ $$ = SingleLangtexCommand($1); }
	;


speakerCommand: 
	SPEAKER_COMMAND optional_parameters OPEN_BRACE content CLOSE_BRACE 
																	{ $$ = SpeakerSemanticAction($2, $4); }
	;

// TODO: see if we can optimize it by adding %empty directly in parameters
optional_parameters:
    parameters  													{ $$ = $1; }
  	| %empty      													{ $$ = EmptyParamList(); }
  	;
	
parameters:
    OPEN_PARENTHESIS param_list CLOSE_PARENTHESIS 					{ $$ = $2; }
	;

param_list:
	param COMMA param_list 											{ $$ = AppendParam($1, $3); }
  	| param                  										{ $$ = SingleParam($1); }
	| %empty      													{ $$ = EmptyParamList(); }
  	;

param:
    ARGS_PARAM EQUAL INTEGER_PARAM 									{ $$ = IntegerParamSemanticAction($1, $3); }
	| ARGS_PARAM EQUAL STRING_PARAM									{ $$ = StringParamSemanticAction($1, $3); }
	| ARGS_PARAM EQUAL BOOLEAN_PARAM								{ $$ = BooleanParamSemanticAction($1, $3); }
  	;

text:
	TEXT 															{ $$ = TextSemanticAction($1); }

%%
