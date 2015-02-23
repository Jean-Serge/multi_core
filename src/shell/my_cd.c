#include "my_cd.h"

/**
 * TODO
 */
int my_cd(char **args, int argc)
{
	int i;
	char *path = NULL;

	/* cd sans argument renvoi à HOME */
	if(argc == 1)
	{
		wd = get_home();
		return SUCCESS;
	}

	/* Sinon on cherche le chemin */
	for(i = 1 ; i < argc ; i++)
	{
		if(*args[i] == '-')
			continue;
		path = args[i];
	}

	if(path == NULL)
	{
		printf("Veuillez indiquer un chemin.\n");
		return PATHNAME_ERROR;
	}

	/* On vérifie qu'il existe */
	if(open_file(NULL, path) == RETURN_FAILURE)
	{
		printf("Le fichier spécifié n'existe pas.\n");
		return IO_ERROR;
	}
	/*	close_file(path);*/
	wd = path;
	return 0;
}
