#ifndef H_MY_CD
#define H_MY_CD

#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "file.h"
#include "return_value.h"

/**
 * Change le répertoire courant en fonction des paramètres.
 */
extern int my_cd(char **args, int argc);

#endif
