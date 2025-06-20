#include "romanize.h"
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#define BASE 0x0410
#define LAST 0x044F

const char *cyrillic_table[LAST - BASE + 1] = {
    "A",  "B",  "V",  "G",  "D",  "E",  "Zh", "Z",  "I",  "Y",   // А-Й
    "K",  "L",  "M",  "N",  "O",  "P",  "R",  "S",  "T",  "U",   // К-У
    "F",  "Kh", "Ts", "Ch", "Sh", "Shch", NULL, "Y", NULL, "E", // Ф-Э
    "Yu", "Ya", // Ю-Я
    "a",  "b",  "v",  "g",  "d",  "e",  "zh", "z",  "i",  "y",   // а-й
    "k",  "l",  "m",  "n",  "o",  "p",  "r",  "s",  "t",  "u",   // к-у
    "f",  "kh", "ts", "ch", "sh", "shch", NULL, "y", NULL, "e", // ф-э
    "yu", "ya"  // ю-я
};

char *decomposeCyrillicChar(uint32_t c) {
    if (c >= BASE && c <= LAST) {  // Covers А-Я and а-я
        char* ret = malloc(5);
        snprintf(ret, 5, "%s", cyrillic_table[c - BASE]);

        return ret;
    }

    if (c < 128) {
        char* ret = malloc(2);
        ret[0] = (char)c;
        ret[1] = '\0';
        return ret;
    }

    return NULL;  // Unsupported character
}

// int main() {
//     setlocale(LC_ALL, "en_US.UTF-8");
//     wchar_t* word = L"В чащах юга жил бы цитрус? Да, но фальшивый экземпляр";
//     char* s = romanizeCyrillic(word);
//     wprintf(L"Input: %ls\nOutput: %s\n", word, s);

//     return 0;
// }