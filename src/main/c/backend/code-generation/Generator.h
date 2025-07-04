#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../romanizers/romanize.h"
// TODO is this ok?
// #include "../../../../references/preamble.tex"
#include "../../frontend/syntactic-analysis/LangTeXAST.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
void generate(char * outputPath, char * fileName, bool isInput, CompilerState *compilerState);

#endif
