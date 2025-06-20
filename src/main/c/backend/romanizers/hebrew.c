#include "romanize.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define BASE 0x05D0
#define LAST 0x05EA
#define MAX_BUF 512

const char *hebrew_table[LAST - BASE + 1] = {
    "",  "b", "g", "d", "h", "v", "z", "kh", "t", "y",   // א - י
    "k", "k", "l", "m", "m", "n", "n", "s", "", "p",     // כ - פ
    "p", "ts", "ts", "k", "r", "sh", "t"                 // צ - ת
};

const char* getHebrewConsonant(uint32_t c) {
    if (c >= BASE && c <= LAST) {
        return hebrew_table[c - BASE];
    }
    return NULL;
}

const char* getHebrewVowel(uint32_t c) {
    switch (c) {
        case 0x05B0: return "e"; // Sheva
        case 0x05B1: case 0x05B2: return "a";
        case 0x05B3: return "o";
        case 0x05B4: return "i";
        case 0x05B5: case 0x05B6: return "e";
        case 0x05B7: case 0x05B8: return "a";
        case 0x05B9: return "o";
        case 0x05BB: return "u";
        default: return NULL;
    }
}

char *decomposeHebrewChar(uint32_t c) {
    const char *cons = getHebrewConsonant(c);
    const char *vowel = getHebrewVowel(c);

    if (cons) {
        char* ret = malloc(6);
        snprintf(ret, 6, "%s", cons);
        return ret;
    }

    if (vowel) {
        char* ret = malloc(4);  
        snprintf(ret, 4, "%s", vowel);
        return ret;
    }

    if (c < 128) {
        char* ret = malloc(2);
        ret[0] = (char)c;
        ret[1] = '\0';
        return ret;
    }

    return NULL;  // Ignore unsupported non-ASCII non-Hebrew characters
}

char* romanizeHebrew(const char* input) {
    if (!input) return NULL;

    char* output = malloc(1024);
    output[0] = '\0';

    const char* ptr = input;
    while (*ptr) {
        const char* saved = ptr;
        uint32_t c1 = utf8ToUnicode(&ptr);

        // Check for ו + ֹ → "o"
        if (c1 == 0x05D5) {
            const char* lookahead = ptr;
            uint32_t c2 = utf8ToUnicode(&lookahead);
            if (c2 == 0x05B9) {
                strcat(output, "o");
                ptr = lookahead;
                continue;
            }
        }

        // Otherwise, use decomposer
        char* romanized = decomposeHebrewChar(c1);
        if (romanized) {
            strcat(output, romanized);
            free(romanized);
        }
    }

    return output;
}

// int main() {
//     setlocale(LC_ALL, "en_US.UTF-8");
//     wchar_t* phrase = L"שָׁלוֹם עוֹלָם";  // "Shalom olam"
//     char* romanized = romanizeHebrew(phrase);
//     if (romanized) {
//         wprintf(L"Input: %ls\nOutput: %s\n", phrase, romanized);
//         free(romanized);
//     } else {
//         printf("Failed to romanize input.\n");
//     }

//     return 0;
// }
