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

 typedef struct Text Text;
 typedef struct Command Command;
 typedef struct LangtexCommand LangtexCommand;
 typedef struct Program Program;
 typedef struct Content Content;
 typedef struct Element Element;
 

 /**
 * Node types for the Abstract Syntax Tree (AST).
 */

//TODO: define the enum types.

enum LangtexCommandType {
    LANGTEX_TRANSLATE,
    LANGTEX_EXERCISE,
    LANGTEX_DIALOGUE,
    LANGTEX_SPEAKER
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
            Content * parameterizedContent;
        };
    };
    CommandType type;
};

struct LangtexCommand{
   union {
    // [!translate] 
    struct {
        Content * leftContent;
        Content * rightContent;
    };
    // [!dialog]
    struct {
        Text * text;
        Content * content;
    };
    // [!exercise]
   };
   LangtexCommandType type;
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

#endif