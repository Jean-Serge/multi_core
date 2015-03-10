#include "context.h"
#include <stdlib.h>

struct ctx_s *ctxs[N_CORE_MAX];

int nb_ctx = 0;

void del_ctx(struct ctx_s *ctx);

/*********************************  Contexte  *********************************/
int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, char **args, int argc){
	ctx->ctx_stack = (char *)malloc(stack_size);
	if(ctx->ctx_stack == NULL){
		return C_RETURN_FAILURE;
	}
	ctx->ctx_stack_size = stack_size;
	ctx->ctx_func = f;
	ctx->ctx_etat = READY;
	ctx->ctx_ebp = ctx->ctx_stack+stack_size-sizeof(int);
	ctx->ctx_esp = ctx->ctx_ebp;
	ctx->ctx_args = args;
	ctx->ctx_argc = argc;
	ctx->ctx_magic = CTX_MAGIC;
	ctx->sem_next = NULL;
	return C_RETURN_SUCCESS;
}

int create_ctx(int stack_size, func_t f, char **args, int argc, char *name){
  return create_ctx_on_core(stack_size, f, args, argc, name, 0);
}


int create_ctx_on_core(int stack_size, func_t f, char **args, int argc, char *name, int core){
	struct ctx_s *ctx;

	irq_disable();
	ctx = (struct ctx_s*)malloc(sizeof(struct ctx_s));
	fprintf(stderr,"Creating context %s (%p)\n", name, ctx);
	ctx->ctx_stack = (char *)malloc(stack_size);
	init_ctx(ctx, stack_size, f, args, argc);
	ctx->ctx_name = name;

	add_ctx(ctx, core);
	nb_ctx++;
	irq_enable();
	return C_RETURN_SUCCESS;
}

/**************************** Ordonancement disque ****************************/
/* void save_drive_ctx(){ */
/* 	irq_disable(); */

/* 	/\* Aucun contexte en cours (normalement impossible) *\/ */
/* 	if(crt_ctx == NULL) */
/* 		{ */
/* 			/\* return; *\/ */
/* 			yield(); */
/* 		} */
/* 	else */
/* 		{ */
/* 			drive_ctx = crt_ctx; */
/* 			/\* if(crt_ctx->ctx_next == crt_ctx) *\/ */
/* 			/\* 	{ *\/ */
/* 			/\* 	  crt_ctx = NULL; *\/ */

/* 			/\* 	} *\/ */
/* 			/\* else *\/ */
/* 			{ */
/* 				struct ctx_s *tmp = crt_ctx; */
/* 				while(tmp->ctx_next != drive_ctx); */
/* 				tmp->ctx_next = drive_ctx->ctx_next; */
/* 			} */
/* 		} */
/* 	irq_enable(); */
/* 	yield(); */
/* } */

/* void switch_to_drive_ctx(){ */
/* 	printf("IRQ disque levée\n"); */
/* 	irq_disable(); */

/* 	if(crt_ctx == NULL){ */
/* 		crt_ctx = drive_ctx; */
/* 		crt_ctx->ctx_next = crt_ctx; */
/* 	} */
/* 	else */
/* 		{ */
/* 			drive_ctx->ctx_next = crt_ctx->ctx_next; */
/* 			crt_ctx->ctx_next = drive_ctx; */
/* 		} */
/* 	drive_ctx = NULL; */
/* 	irq_enable(); */

/* 	yield(); */
/* } */

/******************************  Ordonancement  *******************************/
void yield(){
	struct ctx_s *ctx;
	int nb_terminated;
	int core = _in(CORE_ID);
	
	printf("Yield\n");
	_out(0xF8, 0xFFFFFFFF-20);

	irq_disable();

	nb_terminated = 0;
	fprintf(stderr, "Yield from ctx %s to ctx %s\n", ctxs[core]->ctx_name, ctxs[core]->ctx_next->ctx_name);
	ctx = next_ctx();

	while(ctx != NULL && ctx->ctx_etat == TERMINATED){
		fprintf(stderr, "\n%s is TERMINATED\n", ctx->ctx_name);
		nb_terminated++;
		ctx = ctx->ctx_next;
	}

	while(ctx->ctx_etat == BLOCKED  && ctx != NULL){
		fprintf(stderr,"\n% is BLOCKED!\n", ctxs[core]->ctx_etat);
		ctx = next_ctx();
	}

	/* irq_enable(); */
	switch_to_ctx(ctx);
}

void switch_to_ctx(struct ctx_s *ctx){
	int core = _in(CORE_ID);
	/* irq_disable(); */
	if(ctx == NULL)
		return;

	assert(ctx->ctx_magic == CTX_MAGIC);
	fprintf(stderr, "\nSwitch to ctx %s\n", ctxs[core]->ctx_name);
	fflush(stdout);

	asm("movl %%ebp, %0" : "=r"(ctxs[core]->ctx_ebp));
	asm("movl %%esp, %0" : "=r"(ctxs[core]->ctx_esp));
	ctxs[core] = ctx;
	asm("movl %0, %%esp" :: "r"(ctxs[core]->ctx_esp));
	asm("movl %0, %%ebp" :: "r"(ctxs[core]->ctx_ebp));
	if(ctxs[core]->ctx_etat == READY){
		ctxs[core]->ctx_etat = ACTIVATED;
		irq_enable();
		ctxs[core]->ctx_func(ctxs[core]->ctx_args, ctxs[core]->ctx_argc);
		irq_disable();

		ctxs[core]->ctx_etat = TERMINATED;
		/* fprintf(stderr,"ctx %s is TERMINATED\n", crt_ctx->ctx_name); */
		printf("ctx %s is TERMINATED\n", ctxs[core]->ctx_name);
		del_ctx(ctxs[core]);
		irq_enable();
		yield();
	}
	irq_enable();
}

/*************************  Gestion liste circulaire  *************************/
void add_ctx(struct ctx_s *ctx, int core){
  /* TODO : voir pour rechercher le core le plus utilisé pour répartir 
     la charge lors d'un ajout de contexte */
	irq_disable();
       
	fprintf(stderr, "add ctx %s\n", ctx->ctx_name);
	if(ctxs[core] == NULL){
		ctxs[core] = ctx;
		ctx->ctx_next = ctx;
	}
	else{
		ctx->ctx_next = ctxs[core]->ctx_next;
		ctxs[core]->ctx_next = ctx;
	}

	irq_enable();
}

/* Supprime le context ctx de la liste des contexte.
 */
void del_ctx(struct ctx_s *ctx){
	irq_disable();

	fprintf(stderr, "del ctx %s\n", ctx->ctx_name);
	if(ctx == NULL){  // Pas d'élément dans la liste, ne rien faire
		return;
		irq_enable();
	}
	if(ctx->ctx_next == ctx){  // un élément dans la liste
		/* free(ctx); */
		ctx = NULL;
	}
	else{
		struct ctx_s *suivant = ctx->ctx_next;
		struct ctx_s *precedent = ctx;

		while(precedent->ctx_next != ctx){
			precedent = precedent->ctx_next;
		}

		precedent->ctx_next = suivant;
		ctx = suivant;
	}
	irq_enable();
}

struct ctx_s *next_ctx(){
	struct ctx_s *nxt;
	int core = _in(CORE_ID);

	irq_disable();
	fprintf(stderr, "next ctx of ctx %s", ctxs[core]->ctx_name);
	

	nxt = ctxs[core];
	

	/* fprintf(stderr, "is ctx %s", nxt->ctx_name); */
	irq_enable();
	return nxt;
}

/************************* Gestion des interruptions  *************************/
void start_sched(){
	fprintf(stderr, "***** start sched ******\n");
	yield();
}

/*************************** Gestion des sémaphore  ***************************/
/*
  Initialise une sémaphore sem avec la valeur val.
*/
void sem_init(struct sem_s *sem, int val, char *name){
	irq_disable();
	sem->sem_max_use = val;
	sem->sem_ctx_lst = NULL;
	sem->name = name;
	irq_enable();
}

/*
  Prendre la sémaphore la sémaphore sem.
*/
void sem_down(struct sem_s *sem){
	int core = _in(CORE_ID);
	irq_disable();
	sem->sem_max_use--;
	if(sem->sem_max_use >= 0){  // Si la sémaphore est libre
		irq_enable();
		return;
	}
	// Sinon
	ctxs[core]->ctx_etat = BLOCKED;
	ctxs[core]->sem_next = sem->sem_ctx_lst;
	sem->sem_ctx_lst = ctxs[core];
	irq_enable();
	yield();  // On change de contexte.
}

/*
  Libérer la sémaphore sem.
*/
void sem_up(struct sem_s *sem){
	irq_disable();
	sem->sem_max_use++;
	/* Déblocage de processus */
	if(sem->sem_max_use <= 0){
		struct ctx_s *tmp = sem->sem_ctx_lst;
		tmp->ctx_etat = ACTIVATED;
		sem->sem_ctx_lst = tmp->sem_next;
	}
	irq_enable();
	return;
}
