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
//  typedef enum ObjectType ObjectType;


 typedef struct Text Text;
 typedef struct Command Command;
 typedef struct ContentList ContentList;
 typedef struct LangtexCommand LangtexCommand;
 typedef struct Program Program;
 typedef struct Content Content;
 typedef struct Element Element;

 typedef struct LangtexParam LangtexParam;
 typedef struct LangtexParamList LangtexParamList;
 typedef struct LangtexCommandList LangtexCommandList;

 //rows, speakers, and more

 typedef struct Object Object;

 /**
 * Node types for the Abstract Syntax Tree (AST).
 */

//TODO: define the enum types.


// enum ObjectType {
//     OBJECT_SPEAKER,
//     OBJECT_ROW
// };

enum LangtexParamType {
    STRING_PARAMETER,
    BOOLEAN_PARAMETER,
    INTEGER_PARAMETER
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

 
enum ContentType {
    ELEMENT,
    SEQUENCE
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

struct Text{
    char * text;
};

struct Command{
    union{
        // \command
		struct {
			char * simpleCommand;
		};

        // \begin{...}...\end{...}
        struct {
            Text * environmentLeftText;
            Content * environmentContent;
            Text * environmentRightText;
        };

        // \command{...}
        struct {
            char * parameterizedCommand;
            ContentList * parameterizedContentList;
        };
    };
    CommandType type;
};

struct ContentList{
    Content * content;
    ContentList * next;
};

struct LangtexCommand{
    LangtexParamList * parameters;
   union {
    // [!translate]{}{}
    struct {
        Content * leftContent;
        Content * rightContent;
    };
    // [!dialog]{}{}, table 
        //spekers-->objectList
    struct {
        LangtexCommandList * langtexCommandList;
    };
    // [!speaker]{}
    struct {
        Content * content;
    };
    // [!row]
    struct {
        ContentList * contentList;
    };

    // [!exercise]
    struct {
        LangtexCommand * options; 
        LangtexCommand * answers;
        LangtexCommand * prompt;
    };
   };
   LangtexCommandType type;
};

struct LangtexCommandList{
    LangtexCommand * command;
    LangtexCommandList * next;
};

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

struct Element{
    union{
        LangtexCommand * langtexCommand;
        Command * command;
        Text * text;
    };
    ElementType type;
};

struct Content{
    union{
        Element * element;
        struct {
            Element * sequenceElement;
            Content * sequenceContent;
        };
    };
    ContentType type;
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
#endif