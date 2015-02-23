#include <stdio.h>
#include <stdlib.h>
#include "context.h"

#define N 10000000

struct sem_s mutex, vide, plein;
typedef struct object_s{
	int value;
} object_t;

object_t stack[N];
int cpt = 0;



void produire_objet(object_t* objet){
	objet->value=1;
}
void utiliser_objet(object_t objet){
	objet.value = 0;
}
void mettre_objet(object_t objet){
	cpt++;
	if(cpt>N){
		printf("Oops!\n");
	}
}
void retirer_objet(object_t* objet){
	cpt--;
	if(cpt<0){
		printf("Oops!\n");
	}
}
void producteur(){
	object_t objet;
	while(1){
		printf("produit: %d/%d\n", cpt, vide.sem_max_use);
		produire_objet(&objet);
		sem_down(&vide);
		sem_down(&mutex);
		mettre_objet(objet);
		sem_up(&mutex);
		sem_up(&plein);
	}
}
void consommateur(){
	object_t objet;
	while(1){
		printf("consomme: %d/%d\n", cpt, plein.sem_max_use);
		sem_down(&plein);
		sem_down(&mutex);
		retirer_objet(&objet);
		sem_up(&mutex);
		sem_up(&vide);
		utiliser_objet(objet);
	}
}

int main(int argc, char*argv[]){
	sem_init(&mutex, 1, "mutex");
	sem_init(&vide, N, "vide");
	sem_init(&plein, 0, "plein");
	create_ctx(16384, &producteur, NULL, "producteur");
	create_ctx(16384, &consommateur, NULL, "consommateur");
	start_sched();
	printf("Fin\n");
	exit(EXIT_SUCCESS);
}
