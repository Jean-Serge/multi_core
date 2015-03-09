#ifndef _H_DRIVE
#define _H_DRIVE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/hardware.h"
#include "../include/hw_info.h"
#include "context.h"

/* Code de retour */
#define SUCCESS 0

/***************************** Macro de deboggage *****************************/
#define dmps_v (int cyl, int sect){\
		printf("Lecture cylindre %d secteur %d\t", cyl, sect);	\
		dmps(cyl, sect);\
	}\

/*********** Opération d'I/O d'entier de 2octets dans les registres ***********/
extern void _out_16bits(unsigned int val, unsigned int addr);

extern unsigned int _in_16bits(unsigned int addr);


/********************* Gestion des informations du disque *********************/
struct disk_info_s{
	/* TODO : ajouter le fabricant du disque */
	unsigned int nb_cyl;          /* Le nombre de cylindres du disque */
	unsigned int nb_sec;          /* Le nombre de secteurs du disque */
	unsigned int sec_size;        /* La taille d'un secteur */
};

extern struct disk_info_s *get_disk_info();


/***************************  Utilitaire multiples  ***************************/
extern int is_correct_coordinates(int sec, int cyl);

extern void mkhd();


/*************************  Commande de bas niveau  ***************************/
extern void move_head(unsigned int cyl, unsigned int sec);


/***************** Fonction d'écriture et lecture de secteur ******************/
extern void read_sector(unsigned int cylinder, unsigned int sector,
                        unsigned char *buffer);

extern void write_sector(unsigned int cylinder, unsigned int sector,
                         const unsigned char *buffer);

extern void format_sector(unsigned int cylinder, unsigned int sector,
                          unsigned int nsector, unsigned int value);

#endif
