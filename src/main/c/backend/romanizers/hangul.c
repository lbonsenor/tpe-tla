// Online C compiler to run C program online
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>

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

char* romanizeHangul(wchar_t* input){
    size_t max_output_size = wcslen(input) * 3 + 1;
    char *output = malloc(max_output_size);
    if (!output) return NULL;

    output[0] = '\0';  // Start with empty string
    
    for (size_t i = 0; input[i] != L'\0'; ++i) {
        const char *latin = decomposeSyllable(input[i]);
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
    wchar_t* word = L"안녕히 가세요";
    char* s = romanizeHangul(word);
    wprintf(L"Input: %ls\nOutput: %s\n", word, s);

    return 0;
}
