#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "string.h"

int
main()
{
    String str = {0}, name = {0};
    str_init(&str, "MINA");
    str_init(&name, "MINA ALBERT SAEED");
    printf("str is: " STR_FMT "\n", STR_ARG(str));

    str_reverse(&name);
    printf("name is: " STR_FMT "\n", STR_ARG(name));

    str_reverse(&name);
    printf("name is: " STR_FMT "\n", STR_ARG(name));

    str_append_cstr(&name, "AAAAAAAAAAAAAAAAAA");
    printf("name is: " STR_FMT "\n", STR_ARG(name));

    str_reverse(&name);
    printf("name is: " STR_FMT "\n", STR_ARG(name));

    str_free(&str);
    str_free(&name);
    return 0;
}
