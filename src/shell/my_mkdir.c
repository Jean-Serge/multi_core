#include "my_mkdir.h"

int my_mkdir(char **args, int argc)
{
	char *name;

	if(argc == 1)
	{
		printf("Veuillez indiquer le nom du répertoire à créer.\n");
		return PATHNAME_ERROR;
	}

	name = args[1];

	if(*name != '/')
	{
		printf("Cette commande ne prend en compte que les chemins absolus pour le moment.\n");
		return PATHNAME_ERROR;
	}

	if(create_file(name, DIRECTORY) == RETURN_FAILURE)
	{
		printf("Problème lors de la création du répertoire, vérifiez le chemin d'accès.\n");
		return IO_ERROR;
	}

	return SUCCESS;
}
