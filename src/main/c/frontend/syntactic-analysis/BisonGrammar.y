%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. **/
	int integer;
	Token token;
	char * string;
	boolean boolean;

	/** Non-terminals. **/
	Program * program;
	Element * element;
	Text * text;
	Content * content;
	ContentList * contentList;
	TextList * textList;
	OptionalNewline * optionalNewline;

	/** Latex **/
	Command * command; 

	/** Langtex **/
	LangtexCommand * langtexCommand; 
	LangtexCommandList * langtexCommandList;

	/** Langtex Parameters **/
	LangtexParam * param;
	LangtexParamList * param_list;
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
%destructor { releaseLangtexCommandList($$); } <langtexCommandList>
%destructor { releaseElement($$); } <element>
%destructor { releaseContent($$); } <content>
%destructor { releaseParamList($$); } <param_list>
%destructor { releaseParam($$); } <param>


/** LaTeX Terminals **/
%token <string> COMMAND
%token <token> BEGIN_ENVIRONMENT
%token <token> END_ENVIRONMENT
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <string> TEXT
%token <token> UNKNOWN

/** LaNgTeX Terminals **/
%token <token> TRANSLATE_COMMAND
%token <token> DIALOG_COMMAND
%token <token> SPEAKER_COMMAND
%token <token> TABLE_COMMAND
%token <token> ROW_COMMAND
%token <token> EXERCISE_COMMAND
%token <token> PROMPT_COMMAND
%token <token> OPTIONS_COMMAND
%token <token> ANSWER_COMMAND
%token <token> BLOCK_COMMAND
%token <token> FILL_COMMAND

/** Punctuation **/ 
%token <token> COMMA
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> EQUAL
%token <string> ARGS_PARAM
%token <token> NEWLINE

/** Param types **/
%token <integer> INTEGER_PARAM
%token <string> STRING_PARAM
%token <boolean> BOOLEAN_PARAM

/** LaTeX Non-terminals. */
%type <program> program
%type <content> content
%type <contentList> commandArgs
%type <content> commandParams
%type <element> element
%type <command> command
%type <text> text
%type <optionalNewline> optionalNewline

/** LaNgTeX Non-terminals **/
%type <langtexCommand> langtexCommand speakerCommand rowCommand exercisePrompt exerciseOptions exerciseAnswer
%type <langtexCommandList> speakerCommands rowCommands

/** Params **/
%type <param> param
%type <param_list> param_list
%type <param_list> parameters

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

/* ============================ PROGRAM ============================ */
program: 
	content 														{ $$ = ContentProgramSemanticAction(currentCompilerState(), $1); }
	;

/* =========================== ESSENTIALS ========================== */

content:
	element content  												{ $$ = AppendContentSemanticAction($1, $2); }
	| %empty														{ $$ = NULL; }
	;

element:
	langtexCommand 			 										{ $$ = LangtexCommandElementSemanticAction($1); }
	| command														{ $$ = CommandElementSemanticAction($1); }
	| text															{ $$ = TextElementSemanticAction($1); }
	;

commandArgs:
	OPEN_BRACE content CLOSE_BRACE commandArgs		{ $$ = ContentListSemanticAction($2, $4); }
	| %empty														{ $$ = NULL; }
	;

text:
	TEXT 															{ $$ = TextSemanticAction($1); }
	| NEWLINE														{ $$ = NewlineTextSemanticAction(); }												
	;

optionalNewline:
	NEWLINE optionalNewline											{ $$ = NULL; }
	| %empty														{ $$ = NULL; }
	;

/* ============================= LATEX ============================= */

command:
	BEGIN_ENVIRONMENT optionalNewline OPEN_BRACE text CLOSE_BRACE commandParams commandArgs content END_ENVIRONMENT optionalNewline OPEN_BRACE text CLOSE_BRACE
																	{ $$ = EnvironmentCommandSemanticAction($4, $6, $7, $8, $12); }
	| COMMAND commandArgs
																	{ $$ = ParameterizedCommandSemanticAction($1,$2); }
	;

commandParams:
	OPEN_BRACKET content CLOSE_BRACKET								{ $$ = $2; }
	| %empty														{ $$ = NULL; }

/* ============================ LANGTEX ============================ */

langtexCommand:
	TRANSLATE_COMMAND parameters optionalNewline OPEN_BRACE content CLOSE_BRACE optionalNewline OPEN_BRACE content CLOSE_BRACE  
																	{ $$ = TranslateSemanticAction($2, $5, $9); }
	| DIALOG_COMMAND parameters optionalNewline OPEN_BRACE optionalNewline speakerCommands CLOSE_BRACE
																	{ $$ = LangtexCommandListSemanticAction($2, $6, LANGTEX_DIALOG); }
	| TABLE_COMMAND parameters optionalNewline OPEN_BRACE optionalNewline rowCommands CLOSE_BRACE 
																	{ $$ = LangtexCommandListSemanticAction($2, $6, LANGTEX_TABLE); }
	| EXERCISE_COMMAND parameters optionalNewline OPEN_BRACE optionalNewline exercisePrompt exerciseOptions exerciseAnswer CLOSE_BRACE 
																	{ $$ = ExerciseSemanticAction($2, $6, $7, $8, LANGTEX_EXERCISE); }
	| BLOCK_COMMAND parameters optionalNewline OPEN_BRACE content CLOSE_BRACE
																	{ $$ = LangtexSimpleContentSemanticAction($2, $5, LANGTEX_BLOCK); }
	| FILL_COMMAND
																	{ $$ = FillSemanticAction(LANGTEX_FILL); }
	;

parameters:
    OPEN_PARENTHESIS param_list CLOSE_PARENTHESIS 					{ $$ = $2; }
	| %empty														{ $$ = EmptyParamList(); }
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

	/* ========================= EXERCISE ========================== */
	exerciseAnswer:
		ANSWER_COMMAND parameters commandArgs NEWLINE				{ $$ = LangtexContentListSemanticAction($2,$3, LANGTEX_ANSWERS);}
		;

	exerciseOptions:
		OPTIONS_COMMAND parameters commandArgs NEWLINE 				{ $$ = LangtexContentListSemanticAction($2,$3, LANGTEX_OPTIONS);}
		| %empty													{ $$ = NULL; }
		;

	exercisePrompt:
		PROMPT_COMMAND parameters OPEN_BRACE content CLOSE_BRACE NEWLINE
																	{ $$ = LangtexSimpleContentSemanticAction($2, $4, LANGTEX_PROMPT);}
		;
		
	/* ============================ ROW ============================ */
	rowCommands:
		rowCommand rowCommands 										{ $$ = AppendLangtexComand($1, $2); }
		| rowCommand 												{ $$ = SingleLangtexCommand($1); } 
		;

	rowCommand:
		ROW_COMMAND parameters commandArgs NEWLINE							{ $$ = LangtexContentListSemanticAction($2, $3, LANGTEX_ROW); }
		;

	/* ========================== SPEAKER ========================== */
	speakerCommands:
		speakerCommand speakerCommands								{ $$ = AppendLangtexComand($1, $2); }
		| speakerCommand 											{ $$ = SingleLangtexCommand($1); }
		;

	
	speakerCommand: 
		SPEAKER_COMMAND parameters OPEN_BRACE content CLOSE_BRACE NEWLINE
																	{ $$ = LangtexSimpleContentSemanticAction($2, $4, LANGTEX_SPEAKER); }
		;

%%
