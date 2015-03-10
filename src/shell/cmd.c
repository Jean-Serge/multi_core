#include "cmd.h"

int core = 0;

/**
 * TODO : sécuriser les appels à realloc
 * TODO : voir la fonction strtok_r pour sécuriser
 * en cas de multi-thread (voir pour la suite du cours)
 */
unsigned int split_cmd(char *cmd, char **split, char *delim)
{
	char *token;
	unsigned int i = 1;
	/* int j; */

	split[0] = strtok(cmd, delim);

	/* Stocke chaque élément de la commande dans le tableau split. */
	while((token = strtok(NULL, delim)) != NULL)
	{
		/* Si on dépasse le nombre d'arguments prévu, on réalloue le pointeur */
		if(i >= 10)
			split = (char **)realloc(split, (i+1)*sizeof(char *));

		split[i++] = token;
	}
	return i;
}

int nope(char **string, int nope){
	printf("nope\n");
	return 0;
}

int execute(char *cmd)
{
	int argc;
	char **split = (char **)malloc(sizeof(char *) * 10);
	int isparallele = 0;
	int (*funct_ptr)(char **, int);

	/******************************/
    /* Décommente moi pour tester */
    /******************************/
	funct_ptr = nope;

	if(*cmd == '&'){
		isparallele = 1;
		cmd++;
	}

	argc = split_cmd(cmd, split, " \t\n");
	cmd = split[0];

	if(cmd == NULL)
		return 0;
	if(strcmp(cmd, "ls") == 0)
		printf("ls\n");
		/* funct_ptr = my_ls; */
	else if(strcmp(cmd, "cd") == 0)
		printf("cd\n");
		/* funct_ptr = my_cd; */
	else if(strcmp(cmd,"pwd") == 0)
		printf("pwd\n");
		/* funct_ptr = my_pwd; */
	else if(strcmp(cmd, "mkdir") == 0)
		printf("mkdir\n");
		/* funct_ptr = my_mkdir; */
	else if(strcmp(cmd, "dumps") == 0)
		printf("dumps\n");
		/* funct_ptr = my_dumps; */
	else if(strcmp(cmd, "exit") == 0)
		return EXIT_VALUE;
	else if(strcmp(cmd, "compute") == 0){
		printf("compute\n");
		funct_ptr = my_compute;
	}
	else
	{
		printf("Commande inconnue\n");
		return NOT_FOUND_ERROR;
	}

	if(!isparallele){
		(* funct_ptr)(split, argc);
	}
	else{
		printf("Create task on core %d\n", core);
		create_ctx_on_core(2048, funct_ptr, split, argc, cmd, core);
		core = (core + 1)% N_CORE_MAX;
		if(core == 0){
			core++;
		}
	}

	return 0;
}
