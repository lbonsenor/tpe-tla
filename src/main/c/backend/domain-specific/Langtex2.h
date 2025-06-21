#ifndef LANGTEX_HEADER
#define LANGTEX_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/LangTeXAST.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include <limits.h>

/** Initialize module's internal state. */
void initializeLangtexModule();

/** Shutdown module's internal state. */
void shutdownLangtexModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */
typedef struct {
	boolean succeed;
	int value;
} ComputationResult;

typedef ComputationResult (*BinaryOperator)(const int, const int);

/** Arithmetic operations. */


/**
 * Computes the final value of a mathematical constant.
 */

/**
 * Computes the final value of a mathematical expression.
 */
// ComputationResult computeExpression(Expression * expression);

ComputationResult computeContent(Content * content);
ComputationResult computeElement(Element * element);
ComputationResult computeText(Text * text);
ComputationResult computeLangtexCommand(LangtexCommand * langtexCommand);
ComputationResult computeCommand(Command * command);

ComputationResult computeTextList(TextList * textList);
ComputationResult computeContentList(ContentList * contentList);    

ComputationResult computeLangtexCommandList(LangtexCommandList * LangtexCommandList);
ComputationResult computeParam(LangtexParam* param);
ComputationResult computeParamList(LangtexParamList * paramList);


/**
 * Computes the final value of a mathematical factor.
 */
// ComputationResult computeFactor(Factor * factor);

#endif
