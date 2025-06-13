#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#define BASE 0x0410
#define LAST 0x044F

const char *table[LAST - BASE + 1] = {
    "A",  "B",  "V",  "G",  "D",  "E",  "Zh", "Z",  "I",  "Y",   // А-Й
    "K",  "L",  "M",  "N",  "O",  "P",  "R",  "S",  "T",  "U",   // К-У
    "F",  "Kh", "Ts", "Ch", "Sh", "Shch", NULL, "Y", NULL, "E", // Ф-Э
    "Yu", "Ya", // Ю-Я
    "a",  "b",  "v",  "g",  "d",  "e",  "zh", "z",  "i",  "y",   // а-й
    "k",  "l",  "m",  "n",  "o",  "p",  "r",  "s",  "t",  "u",   // к-у
    "f",  "kh", "ts", "ch", "sh", "shch", NULL, "y", NULL, "e", // ф-э
    "yu", "ya"  // ю-я
};

const char *romanizeCyrillicChar(wchar_t c) {
    if (c >= BASE && c <= LAST) {  // Covers А-Я and а-я
        return table[c - BASE];
    }
    return NULL;  // Unsupported character
}

char *romanizeCyrillic(const wchar_t *input) {
    // Estimate size: max 4 Latin chars per Cyrillic letter
    size_t max_output_size = wcslen(input) * 4 + 1;
    char *output = malloc(max_output_size);
    if (!output) return NULL;

    output[0] = '\0';  // Start with empty string

    for (size_t i = 0; input[i] != L'\0'; ++i) {
        const char *latin = romanizeCyrillicChar(input[i]);
        if (latin) {
            strcat(output, latin);
        } else {
            // For non-Cyrillic characters, just copy as-is (if ASCII)
            if (input[i] < 128) {
                char ascii[2] = { (char)input[i], '\0' };
                strcat(output, ascii);
            }
        }
    }

    return output;
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    wchar_t* word = L"В чащах юга жил бы цитрус? Да, но фальшивый экземпляр";
    char* s = romanizeCyrillic(word);
    wprintf(L"Input: %ls\nOutput: %s\n", word, s);

    return 0;
}