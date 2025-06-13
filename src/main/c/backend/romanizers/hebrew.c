#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define BASE 0x05D0
#define LAST 0x05EA
#define MAX_BUF 512

const char *table[LAST - BASE + 1] = {
    "",  "b", "g", "d", "h", "v", "z", "kh", "t", "y",   // א - י
    "k", "k", "l", "m", "m", "n", "n", "s", "", "p",     // כ - פ
    "p", "ts", "ts", "k", "r", "sh", "t"                 // צ - ת
};

const char* get_hebrew_consonant(wchar_t c) {
    if (c >= BASE && c <= LAST) {
        return table[c - BASE];
    }
    return NULL;
}

const char* get_hebrew_vowel(wchar_t c) {
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

char* romanizeHebrew(const wchar_t* input) {
    if (!input) return NULL;

    size_t len = wcslen(input);
    char *output = malloc(MAX_BUF);
    if (!output) return NULL;
    output[0] = '\0';

    wchar_t base = 0;
    const char *base_trans = NULL;

    for (size_t i = 0; i < len; i++) {
        wchar_t c = input[i];

        // Check for base letter
        const char* translit = get_hebrew_consonant(c);
        if (translit) {
            base = c;
            base_trans = translit;

            // Special case: Shin or Sin dots
            if (i + 1 < len && input[i + 1] == 0x05C1) { // Shin dot
                base_trans = "sh";
                i++;
            } else if (i + 1 < len && input[i + 1] == 0x05C2) { // Sin dot
                base_trans = "s";
                i++;
            }

            // Check for Holam male: ו + ֹ = "o"
            if (c == 0x05D5 && i + 1 < len && input[i + 1] == 0x05B9) {
                strcat(output, "o");
                i++;
                continue;
            }

            strcat(output, base_trans);
            continue;
        }

        // Handle niqqud
        const char* vowel = get_hebrew_vowel(c);
        if (vowel) {
            strcat(output, vowel);
            continue;
        }

        // ASCII pass-through
        if (c < 128) {
            char ascii[2] = { (char)c, '\0' };
            strcat(output, ascii);
        }
    }

    return output;
}

int main() {
    setlocale(LC_ALL, "");

    wchar_t* phrase = L"שָׁלוֹם עוֹלָם";  // "Shalom olam"
    char* romanized = romanizeHebrew(phrase);
    if (romanized) {
        wprintf(L"Input: %ls\nOutput: %s\n", phrase, romanized);
        free(romanized);
    } else {
        printf("Failed to romanize input.\n");
    }

    return 0;
}
