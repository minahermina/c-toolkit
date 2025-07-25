#include <stdio.h>
#include <string.h>
#include "string.h"

int
main()
{
    String str = {0}, name = {0};
    str_init(&str, "MINA");
    printf("str is: " STR_FMT "\n", STR_ARG(str));
    
    str_lower(&str);
    printf("str is: " STR_FMT "\n", STR_ARG(str));

    str_upper(&str);
    printf("str is: " STR_FMT "\n", STR_ARG(str));

    char ch;
    STR_FOREACH(str, ch){
        printf("%c", ch);
    }

    str_free(&str);
    return 0;
}
