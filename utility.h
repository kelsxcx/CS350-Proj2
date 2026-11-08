#ifndef UTILITY_H
#define UTILITY_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ask(const char *prompt);
int confirm(const char *prompt);
void press_enter(void);

#endif
