#ifndef H_CONTEXT
#define H_CONTEXT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "hw.h"
#include "hw_info.h"

#define TERMINATED 0 
#define READY      1 /* changement de context, pas d'appel de fonction */
#define ACTIVATED  2 /* changement de context + appel de fonction */
#define BLOCKED    3 /* Blocked by a semaphore */
#define CTX_MAGIC 0xB0B0A1E

#define C_RETURN_FAILURE 1
#define C_RETURN_SUCCESS 0

struct ctx_s *drive_ctx;

#define STACK_SIZE 20000

#define dump_sp()\
	do{\
		void *esp;\
		void *ebp;\
		asm("movl %%ebp, %0" : "=r"(ebp));\
		asm("movl %%esp, %0" : "=r"(esp));\
		printf("$ebp = %p\t$esp = %p\n", ebp, esp);\
	}while(0);

typedef int(func_t)(char **, int);

/*********************************  Contexte  *********************************/
struct ctx_s {
	char *ctx_esp;
	char *ctx_ebp;
	char *ctx_stack;
	int ctx_stack_size;
	func_t *ctx_func;
	char **ctx_args;
	int ctx_argc;
	unsigned int size;
	int ctx_etat;
	int ctx_magic;
	char *ctx_name;
	struct ctx_s *ctx_next;
	struct ctx_s *sem_next;
};

extern int create_ctx(int stack_size, func_t f, char **args, int argc, char *name);

/**************************** Ordonancement disque ****************************/
/**
   Sauvegarde le contexte courant dans drive_ctx. Déclenche un changement de
   contexte.
*/
extern void save_drive_ctx();
/**
   Fait un appel à switch_to_ctx avec comme paramêtre drive_ctx.
   Réinitialise cette dernière.
*/
extern void switch_to_drive_ctx();

/******************************  Ordonancement  *******************************/
extern void switch_to_ctx(struct ctx_s *ctx);
extern void yield();

/*************************  Gestion liste circulaire  *************************/
extern void add_ctx(struct ctx_s *ctx, int core);
struct ctx_s *next_ctx();

/************************* Gestion des interruptions  *************************/
extern void start_sched();

/*************************** Gestion des sémaphore  ***************************/
struct sem_s{
	int sem_max_use;
	struct ctx_s *sem_ctx_lst;
	char *name;
};

/************************** Gestion des sémaphores  ***************************/
extern void sem_init(struct sem_s *sem, int val, char *name);
extern void sem_down(struct sem_s *sem);
extern void sem_up(struct sem_s *sem);

#endif
