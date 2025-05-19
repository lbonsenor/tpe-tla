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
    
    /*
    wchar_t initial_consonant[] =  	{L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', 
                                    	 L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', 
                                    	 L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ', 
                                    	 L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', 
                                    	 L'ㅌ', L'ㅍ', L'ㅎ'};
                                    
    wchar_t medial_vowel[] =   	{L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', 
                                    	 L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', 
                                    	 L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ', 
                                    	 L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', 
                                    	 L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', 
                                    	 L'ㅣ'};
                                    
    wchar_t final_consonant[] =	{L' ', L'ㄱ', L'ㄲ', L'ㄳ', 
                                    	 L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', 
                                    	 L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ', 
                                    	 L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', 
                                    	 L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ', 
                                    	 L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', 
                                    	 L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ',};
    */

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
    

    char* ret = (char*)malloc(7 * sizeof(char));;
    if (!ret) return NULL;

    snprintf(ret, 7, "%s%s%s", initials[i_index], medials[m_index], finals[f_index]);

    return ret;
}

char* romanizeKR(wchar_t* word){
    char* ret = malloc(BLOCK * sizeof(char));
    int total_length = 0;
    int total_size = 10;
    
    for (int i = 0; word[i] != L'\0'; i++){
        char* aux = decomposeSyllable(word[i]);
        if (aux){
            total_length += strlen(aux);
            if (total_length >= total_size){
                total_size += BLOCK;
                ret = realloc(ret, total_size);
            }
            strcat(ret, aux);
            free(aux);
        }
    }
    
    return ret;
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    wchar_t* word = L"안녕히 가세요";
    char* s = romanizeKR(word);
    wprintf(L"Input: %ls\nOutput: %s\n", word, s);

    return 0;
}
