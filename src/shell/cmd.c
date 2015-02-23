#include "cmd.h"

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

	/* Décommenter pour afficher le résultat du split */
	/* for(j = 0 ; j < i ; j++) */
	/*   printf("%s\n", split[j]); */

	return i;
}

int execute(char *cmd)
{
	int argc;
	char **split = (char **)malloc(sizeof(char *) * 10);
	int isparallele = 0;
	int (*funct_ptr)(char **, int);

	if(*cmd == '&'){
		isparallele = 1;
		cmd++;
	}

	argc = split_cmd(cmd, split, " \t\n");
	cmd = split[0];

	if(cmd == NULL)
		return 0;
	if(strcmp(cmd, "ls") == 0)
		funct_ptr = my_ls;
	else if(strcmp(cmd, "cd") == 0)
		funct_ptr = my_cd;
	else if(strcmp(cmd,"pwd") == 0)
		funct_ptr = my_pwd;
	else if(strcmp(cmd, "mkdir") == 0)
		funct_ptr = my_mkdir;
	else if(strcmp(cmd, "dumps") == 0)
		funct_ptr = my_dumps;
	else if(strcmp(cmd, "exit") == 0)
	  return EXIT_VALUE;
	else if(strcmp(cmd, "compute") == 0)
	  funct_ptr = my_compute;
	else
	{
		printf("Commande inconnue\n");
		return NOT_FOUND_ERROR;
	}

	if(!isparallele){
		(* funct_ptr)(split, argc);
	}
	else{
		create_ctx(2048, funct_ptr, split, argc, cmd);
	}

	return 0;
}
