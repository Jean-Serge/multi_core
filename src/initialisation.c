#include "initialisation.h"

/*
  Fonction ne faissant rien. Utile pour initialiser les fonctions appelé lors
  des interruptions du matériel.
 */
void empty_fun(){
	while(1);
}

void function(){
	int n_core = _in(CORE_ID);
	int i;
	printf("Core : %d\n", n_core);
	create_ctx_on_core(200000, empty_fun, NULL, 0, "empty_fun", n_core);
	yield();
}

/*
  Initialise le simulateur de matérielle.
 */
void mkhd(){
	int i;

	/* Initialisation du matériel */
	if(init_hardware("etc/hardware.ini") == 0){
		printf("Erreur lors de l'initialisation du matériel.\n");
		exit(HW_INIT_ERROR);
	}


	irq_disable();
	/* Association de handlers à toutes les IRQ */
	for(i = 0; i < NB_HW_IRQ; i++){
		IRQVECTOR[i] = empty_fun;
	}
	IRQVECTOR[0] = function;
	IRQVECTOR[TIMER_IRQ] = yield;



	/* IRQVECTOR[HDA_IRQ] = &switch_to_drive_ctx; */

	_out(0xF4, 128+64+32+8);
	_out(0xF8, 0xFFFFFFFF - 20);

	/* create_ctx(65536, empty, NULL, 0, "vide"); */

	for(i = 1; i < N_CORE_MAX; i++){
		_out(CORE_IRQMAPPER+i, 0xF);
	}
	_out(CORE_IRQMAPPER, 0x0);

	/* On active les 4 premiers core 0xF -> 1111 */
	_out(CORE_STATUS, 0xF);

	irq_enable();
	/* _mask(1); */
	return;
}
