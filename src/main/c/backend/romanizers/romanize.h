#ifndef ROMANIZE_H
#define ROMANIZE_H

#include <wchar.h>
#include <locale.h>
#include <stdint.h>

char* romanize(const char* lang, const char * input);
char* romanizeHebrew(const char* input);
char* decomposeHangulChar(uint32_t syllable);
char* decomposeCyrillicChar(uint32_t c);

uint32_t utf8ToUnicode(const char** str);

#endif