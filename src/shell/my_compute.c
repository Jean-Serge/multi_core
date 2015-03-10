#include "my_compute.h"

int my_compute(char **args, int argc)
{
  int i;

  printf("Début du compute.\n");fflush(stdout);
  for(i = 0 ; i < 2000000000 ; i++){
      /* fprintf(stderr,"i -> %d\n", i);fflush(stdout); */
  }
  printf("Fin du compute.\n");fflush(stdout);

  return SUCCESS;
}
