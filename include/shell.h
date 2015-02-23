#ifndef H_SHELL
#define H_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "cmd.h"
#include "context.h"
#include "return_value.h"

#define CMD_SIZE 101

char *wd;

extern char *get_home();

#endif
