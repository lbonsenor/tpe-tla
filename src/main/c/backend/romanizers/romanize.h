#ifndef ROMANIZE_H
#define ROMANIZE_H

#include <wchar.h>
#include <locale.h>

char* romanizeHangul(const char* utf8_input);
// char* romanizeHangul(const wchar_t* input);
char* romanizeHebrew(const wchar_t* input);
char* romanizeCyrillic(const wchar_t* input);

#endif