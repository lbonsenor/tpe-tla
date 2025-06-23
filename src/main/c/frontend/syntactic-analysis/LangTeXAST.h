#ifndef LANGTEXAST_HEADER
#define LANGTEXAST_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeRenameMeModule();

/** Shutdown module's internal state. */
void shutdownRenameMeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ContentType ContentType;
typedef enum ElementType ElementType;
typedef enum CommandType CommandType;
typedef enum LangtexCommandType LangtexCommandType;
typedef enum LangtexParamType LangtexParamType;

typedef struct Text Text;
typedef struct Command Command;
typedef struct ContentList ContentList;
typedef struct TextList TextList;
typedef struct LangtexCommand LangtexCommand;
typedef struct Program Program;
typedef struct Content Content;
typedef struct Element Element;
typedef struct Text OptionalNewline;

typedef struct LangtexParam LangtexParam;
typedef struct LangtexParamList LangtexParamList;
typedef struct LangtexCommandList LangtexCommandList;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */
enum ContentType {
    ELEMENT,
    SEQUENCE
};

enum CommandType {
    SIMPLE,
    PARAMETERIZED,
    ENVIRONMENT
};

enum ElementType {
    LANGTEX_COMMAND,
    LATEX_COMMAND,
    LATEX_TEXT
};

enum LangtexCommandType {
    LANGTEX_TRANSLATE,
    LANGTEX_EXERCISE,
    LANGTEX_DIALOG,
    LANGTEX_SPEAKER,
    LANGTEX_TABLE,
    LANGTEX_ROW,
    LANGTEX_OPTIONS,
    LANGTEX_ANSWERS,
    LANGTEX_PROMPT,
    LANGTEX_BLOCK,
    LANGTEX_LANGUAGE,
    LANGTEX_FILL
};

enum LangtexParamType {
    STRING_PARAMETER,
    BOOLEAN_PARAMETER,
    INTEGER_PARAMETER
};

/* Langtex */

struct LangtexCommand{
    LangtexParamList * parameters;
    union {
    // Usage: translate
    struct {
        Content * leftText;
        Content * rightText;
    };
    // Usage: dialog, hebrew_table
    struct {
        LangtexCommandList * langtexCommandList;
    };
    // Usage: speaker,prompt
    struct {
        Content * content;
    };
    // Usage: row, answers, options

    struct {
        ContentList * contentList;
    };
    // Usage: exercise
    struct {
        LangtexCommand * options;
        LangtexCommand * answers;
        LangtexCommand * prompt;
    };
    // Usage: language
    struct {
        TextList * textList;
    };
    // Usage: fill
    struct {
        Text * text;
    };
   };
   LangtexCommandType type;
};

struct LangtexCommandList{
    LangtexCommand * command;
    LangtexCommandList * next;
};

struct Prompt{
    Content * promptContent;
    LangtexParamList * parameters;
    LangtexCommandType type;
};

struct Options{
    ContentList * contentList;
    LangtexParamList * parameters;
    LangtexCommandType type;
};

struct Answer{
    ContentList * contentList;
    LangtexParamList * parameters;
};

/* Langtex Parameters */
// [!translate]{lang="kr"}
struct LangtexParam {
    char * key;
    union {
        char * stringParam;
        int intParam;
        boolean boolParam;
    } value;
    LangtexParamType type;
};
    
struct LangtexParamList {
    LangtexParam * param;
    LangtexParamList * next;
};

/* Latex */

struct Content{
    union{
        // Element * element;
        struct {
            Element * sequenceElement;
            Content * sequenceContent;
        };
    };
    ContentType type;
};

struct ContentList{
    Content * content;
    ContentList * next;
};

struct Text{
    char * text;
};

struct TextList{
    Text * text;
    TextList * next;
};

struct Command{
    union{
        // \command
		// struct {
		// 	char * simpleCommand;
		// };

        // \begin{...}...\end{...} -> environment
        struct {
            Text * environmentLeftText;
            Content * environmentParameters;
            ContentList * environmentCommandArgs;
            Content * environmentContent;
        };

        // \command{...} -> parameterized
        struct {
            char * parameterizedCommand;
            ContentList * parameterizedContentList;
        };
    };
    CommandType type;
};

struct Element{
    union{
        LangtexCommand * langtexCommand;
        Command * command;
        Text * text;
    };
    ElementType type;
};

struct Program {
	Content * content;
};


/**
 * Node recursive destructors.
 */
 void releaseText(Text * text);
 void releaseCommand(Command * command);
 void releaseElement(Element * Element);
 void releaseContent(Content * content);
 void releaseProgram(Program * program);
 void releaseLangtexCommand(LangtexCommand * langtexCommand);
 void releaseLangtexCommandList(LangtexCommandList * langtexCommandList);
 void releaseParam(LangtexParam * param);
 void releaseParamList(LangtexParamList * list);
 void releaseObject(LangtexCommandList * langtexCommandList);
 void releaseContentList(ContentList * contentList);
 void releaseTextList(TextList * textList);
#endif