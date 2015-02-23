#include "hardware.h"
#include "hw_info.h"

#include <stdio.h>
#include <stdlib.h>

void function(){
	int i;
	_mask(1);
	while(1){
		printf("core %d\n", _in(CORE_ID));
		for(i = 0; i < 1<<25; i++){}
	}
}

void print_IRQ(){
	printf("## Intéruption %d ##\n", _in(CORE_ID));fflush(stdout);
	_out(TIMER_ALARM, 0xFFFFFFFF - 100);
}

void empty_irq(){
	return;
}

int main(int argc, char *argv[]){
	int i;
	if(init_hardware("etc/hardware.ini") == 0){
		printf("Erreur dans l'initialisation du hardware.\n");
		return 1;
	}

	for(i = 1; i < 16; i++)
		IRQVECTOR[i] = empty_irq;

	IRQVECTOR[0] = function;

	/* On active les 4 premiers core 0xF -> 1111 */
	_out(CORE_STATUS, 0x3F);

	IRQVECTOR[TIMER_IRQ] = print_IRQ;
	for(i = 0; i < 6; i++)
		_out(CORE_IRQMAPPER+i, 0);

	_out(CORE_IRQMAPPER+2, 1<< TIMER_IRQ);/* On redirige l'IRQ TIMER vers le core 2 */

	_out(TIMER_PARAM, 128 + 64 /* + 32 +8 */);
	_out(TIMER_ALARM, 0xFFFFFFFF - 20);

	/* _mask(1); */
	function();

	return 0;
}
