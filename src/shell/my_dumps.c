#include "my_dumps.h"


int my_dumps(char **args, int argc){
	int secteur = -1;
	unsigned char *buf;
	if(argc != 2){
		printf("Pas assez d'argument.\n");
		return 1;
	}
	secteur = atoi(*args);
	if(secteur == -1){
		printf("Erreur lors de la conversion du numéro de bloc.\n");
		return 1;
	}
	buf = (unsigned char *)malloc(sizeof(unsigned char) * HDA_SECTORSIZE);

	read_bloc(vol_courant, secteur, buf);

	for(secteur = 0; secteur < HDA_SECTORSIZE; secteur++){
		printf("%hx ", buf[secteur]);
		if(secteur % 20 == 0){
			printf("\n");
		}
	}
	return 0;
}
