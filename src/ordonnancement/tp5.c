#include "context.h"
#include <stdio.h>

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;


void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char *argv[]){
	create_ctx(STACK_SIZE, f_ping, NULL, "f_ping");
	create_ctx(STACK_SIZE, f_pong, NULL, "f_pong");
	start_sched();
	exit(EXIT_SUCCESS);
}

void f_ping(void *args){
	int i;
	for(i = 0; i < 10000; i++) {
		printf("A");
		fflush(stdout);
		printf("B");
		fflush(stdout);
		printf("C");
		fflush(stdout);
	}
}

void f_pong(void *args){
	int i;
	for(i = 0; i < 10000; i++) {
		printf("1");
		fflush(stdout);
		printf("2");
		fflush(stdout);
	}
}
