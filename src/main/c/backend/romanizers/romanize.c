#include "romanize.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

char *romanize(const char *lang, const char *input);
char *genericRomanize(const char* input, size_t max_char_size, char *(*decomposer)(uint32_t));

char *romanize(const char *lang, const char *input)
{
    if (input == NULL)
        return " ";

    if (strcmp(lang, "kr") == 0)
    {
        return genericRomanize(input, 7, &decomposeHangulChar);
    }
    else if (strcmp(lang, "heb") == 0)
    {
        // Hebrew is too complex and it needs it's own romanizer
        return romanizeHebrew(input);
    }
    else if (strcmp(lang, "ru") == 0)
    {
        return genericRomanize(input, 4, &decomposeCyrillicChar);
    }
    else
    {
        // Unsupported language - this is an error
        return " ";
    }
}

uint32_t utf8ToUnicode(const char** str) {
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

char *genericRomanize(const char* input, size_t max_char_size, char *(*decomposer)(uint32_t)){
    if (!input) return NULL;
    
    size_t input_len = strlen(input);
    size_t max_output_size = input_len * max_char_size + 1;
    char* output = malloc(max_output_size);
    if (!output) return NULL;
    
    output[0] = '\0';
    
    const char* ptr = input;
    
    while (*ptr) {
        uint32_t codepoint = utf8ToUnicode(&ptr);
        
        if (codepoint == 0) continue; // Skip invalid bytes
        
        char* romanized = decomposer(codepoint);
        if (romanized) {
            strcat(output, romanized);
            free(romanized);
        }
    }
    
    return output;
}
