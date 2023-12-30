#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definitions.h"

void InputString(char* input, const char* format) {
    char buffer[256];
    scanf(format, buffer);
    strcpy(input, buffer);
    CLR_BUF;
}
