/* ------------------------------
   $Id: hw.c,v 1.5 2006/10/24 07:21:59 marquet Exp $
   ------------------------------------------------------------

   Basic hardware emulation.
   Philippe Marquet, Dec 2003

*/

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <assert.h>

#include "hw.h"

/* a 8 milliseconds timer */
#define TIMER_SEC  0
#define TIMER_USEC 8000


void irq_disable(){
	enable = 0;
	_mask(3);
}

void irq_enable(){
	enable = 1;
	_mask(1);
}
