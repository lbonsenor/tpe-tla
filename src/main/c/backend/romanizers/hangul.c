// Online C compiler to run C program online
#include "romanize.h"
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define BASE 0xAC00
#define LAST 0xD7A3
#define BASE_JAMO 0x3130
#define LAST_JAMO 0x318F
#define BLOCK 10

#define INITIAL_COUNT 19
#define MEDIAL_COUNT 21
#define FINAL_COUNT 28

const char *initials[] =  { "g", "kk", "n", "d", 
                            "tt", "r/l", "m", "b", 
                            "bb", "s", "ss", "", 
                            "j", "jj", "ch", "k", 
                            "t", "p", "h" };

const char *medials[] =   { "a", "ae", "ya", "yae", 
                            "eo", "e", "yeo", "ye", 
                            "o", "wa", "wae", "oe", 
                            "yo", "u", "wo", "we", 
                            "wi", "yu", "eu", "ui", 
                            "i" };

const char *finals[] =    { "", "k", "kk", "ks",  
                            "n", "nj", "nh", "t",  
                            "l", "lk", "lm", "lb",  
                            "ls", "lt", "lp", "lh",  
                            "m", "p", "ps", "s",  
                            "ss", "ng", "j", "ch",  
                            "k", "t", "p", "h" };
                            
char* decomposeSyllable(wchar_t syllable){
    if (syllable < BASE || syllable > LAST){
        char* ret = (char*)malloc(2 * sizeof(char));
        ret[0] = (char)syllable;
        ret[1] = '\0';
        return ret;
    }

    int offset = syllable - BASE;
    
    int i_index = offset / (MEDIAL_COUNT * FINAL_COUNT);
    int m_index = (offset % (MEDIAL_COUNT * FINAL_COUNT)) / FINAL_COUNT;
    int f_index = offset % FINAL_COUNT;

    

    char* ret = (char*)malloc(7 * sizeof(char));;
    if (!ret) return NULL;

    snprintf(ret, 7, "%s%s%s", initials[i_index], medials[m_index], finals[f_index]);

    return ret;
}

// char* romanizeHangul(const wchar_t* input){
//     size_t max_output_size = wcslen(input) * 3 + 1;
//     char *output = malloc(max_output_size);
//     if (!output) return NULL;

//     output[0] = '\0';  // Start with empty string
    
//     for (size_t i = 0; input[i] != L'\0'; ++i) {
//         char *latin = decomposeSyllable(input[i]);
//         if (latin) {
//             strcat(output, latin);
//             free(latin);
//         } else {
//             // For non-Cyrillic characters, just copy as-is (if ASCII)
//             if (input[i] < 128) {
//                 char ascii[2] = { (char)input[i], '\0' };
//                 strcat(output, ascii);
//             }
//         }
//     }
    
//     return output;
// }

char* decomposeSyllable_utf32(uint32_t syllable) {
    if (syllable < BASE || syllable > LAST) {
        // Para caracteres no-hangul, solo crea string si es ASCII
        if (syllable < 128) {
            char* ret = malloc(2);
            ret[0] = (char)syllable;
            ret[1] = '\0';
            return ret;
        }
        return NULL;
    }

    int offset = syllable - BASE;
    
    int i_index = offset / (MEDIAL_COUNT * FINAL_COUNT);
    int m_index = (offset % (MEDIAL_COUNT * FINAL_COUNT)) / FINAL_COUNT;
    int f_index = offset % FINAL_COUNT;

    char* ret = malloc(7);
    if (!ret) return NULL;

    snprintf(ret, 7, "%s%s%s", initials[i_index], medials[m_index], finals[f_index]);

    return ret;
}


uint32_t utf8_to_unicode(const char** str) {
    const unsigned char* s = (const unsigned char*)*str;
    uint32_t codepoint = 0;
    
    if (s[0] < 0x80) {
        // ASCII (1 byte)
        codepoint = s[0];
        *str += 1;
    } else if ((s[0] & 0xE0) == 0xC0) {
        // 2 bytes
        codepoint = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
        *str += 2;
    } else if ((s[0] & 0xF0) == 0xE0) {
        // 3 bytes (aquí están los caracteres coreanos)
        codepoint = ((s[0] & 0x0F) << 12) | 
                   ((s[1] & 0x3F) << 6) | 
                   (s[2] & 0x3F);
        *str += 3;
    } else if ((s[0] & 0xF8) == 0xF0) {
        // 4 bytes
        codepoint = ((s[0] & 0x07) << 18) | 
                   ((s[1] & 0x3F) << 12) | 
                   ((s[2] & 0x3F) << 6) | 
                   (s[3] & 0x3F);
        *str += 4;
    } else {
        // Byte inválido
        *str += 1;
        return 0;
    }
    
    return codepoint;
}

char* romanizeHangul(const char* utf8_input) {
    if (!utf8_input) return NULL;
    
    size_t input_len = strlen(utf8_input);
    size_t max_output_size = input_len * 3 + 1;
    char* output = malloc(max_output_size);
    if (!output) return NULL;
    
    output[0] = '\0';
    
    const char* ptr = utf8_input;
    
    while (*ptr) {
        uint32_t codepoint = utf8_to_unicode(&ptr);
        
        if (codepoint == 0) continue; // Skip invalid bytes
        
        char* romanized = decomposeSyllable_utf32(codepoint);
        if (romanized) {
            strcat(output, romanized);
            free(romanized);
        }
    }
    
    return output;
}

// int main() {
//     setlocale(LC_ALL, "en_US.UTF-8");
//     wchar_t* word = L"안녕히 가세요";
//     char* s = romanizeHangul(word);
//     wprintf(L"Input: %ls\nOutput: %s\n", word, s);

//     return 0;
// }
