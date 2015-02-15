#include "hardware.h"
#include "hw_info.h"

#include <stdio.h>

void function(){
	int i;
	while(1){
		printf("%d\n", _in(CORE_ID));
		for(i = 0; i < 100000000; i++){}
	}
}

int main(int argc, char *argv[]){
	if(init_hardware("etc/core.ini") == 0){
		printf("Erreur dans l'initialisation du hardware.\n");
		return 1;
	}

	IRQVECTOR[0] = function;

	_mask(1);

	/* On active les 4 premiers core 0xF -> 1111 */
	_out(CORE_STATUS, 0xF);

	function();
	return 0;
}
