#include <stdio.h>
#include <string.h>
#include "string.h"

int
main()
{
    String str = {0};
    str_init(&str, "Mina");
    // printf("%s\n", str.chars);

    debug_string(&str);
    str_append_cstr(&str," Albert Saeed");
    printf("My name is " STR_FMT "\n\n", STR_ARG(str));
    debug_string(&str);
    return 0;
}
