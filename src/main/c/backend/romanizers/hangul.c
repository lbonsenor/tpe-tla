// Online C compiler to run C program online
#include "romanize.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE 0xAC00
#define LAST 0xD7A3
#define BASE_JAMO 0x3130
#define LAST_JAMO 0x318F
#define MAX_CHAR_SIZE 3

#define INITIAL_COUNT 19
#define MEDIAL_COUNT 21
#define FINAL_COUNT 28

const char *initials[] =  { "g", "kk", "n", "d",          
                            "tt", "r", "m", "b", 
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

char* decomposeHangulChar(uint32_t syllable) {
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
    
    char* ret = malloc(8);
    if (!ret) return NULL;
    
    snprintf(ret, 8, "%s%s%s", initials[i_index], medials[m_index], finals[f_index]);
    
    return ret;
}