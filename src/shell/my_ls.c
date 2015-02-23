#include "my_ls.h"

/**
 * TODO
 */
int my_ls(char **args, int argc)
{
	file_desc_t *fd = (file_desc_t *)malloc(sizeof(file_desc_t));
	char *path;
	_mask(10);
	printf("J'affiche le contenu du répertoire.\n");
	if(argc == 1)
	  path = wd;
	else
	  path = args[1];

	if(open_file(fd, path) == RETURN_FAILURE)
	  {
	    printf("Le fichier spécifié n'existe pas.\n");
	    return IO_ERROR;
	  }

	print_fd(fd);
	_mask(0);
	return 0;
}
