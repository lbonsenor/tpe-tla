#include "romanize.h"
#include <string.h>

char *romanize(const char *lang, const char *input)
{
    if (input == NULL)
        return " ";

    if (strcmp(lang, "kr") == 0)
    {
        return romanizeHangul(input);
    }
    // else if (strcmp(lang, "hebrew") == 0)
    // {
    //     return romanizeHebrew(input);
    // }
    // else if (strcmp(lang, "cyrillic") == 0)
    // {
    //     return romanizeCyrillic(input);
    // }
    else
    {
        // Unsupported language - this is an error
        return NULL;
    }
}