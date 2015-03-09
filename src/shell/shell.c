#include "shell.h"

char prompt[3] = ">> ";


char *get_home()
{
	return "/";
}

/**
 * Lit au plus size - 1 caractères sur l'entrée standard et les
 * écrit dans cmd, ignore les caractères suivant.
 * Si une ligne entrée fait plus de size caractères, cette fonction
 * ignorera les caractères en plus et ne les considérera pas comme une
 * autre chaîne.
 */
void my_fgets(char *cmd, unsigned int size)
{
	int i;
	char c;
	/* On lit CMD_SIZE - 1 caractères sur l'entrée standard. */
	for(i = 0 ; i < size - 1 ; i++)
	{
		c = getchar();

		if(c == '\n' || c == EOF)
		{
			cmd[i] = '\0';
			return;
		}

		cmd[i] = c;
	}

	cmd[i+1] = '\0';

	/* Ignore les caractère suivant. */
	while(1)
	{
		c = getchar();
		if(c == EOF || c == '\n')
			return;
	}
}


int shell(char **argc, int i){
	char *cmd;
	int isexit;
	int ret_val;
	isexit = 0;

	while(1 && !isexit)
	{
		ret_val = NOT_FOUND_ERROR;
		cmd = readline(prompt);

		if(cmd != NULL)               /* En cas de ctrl-D */
			ret_val = execute(cmd);
		else
			putchar('\n');

		if(ret_val == EXIT_VALUE)
			isexit = 1;

		if(ret_val != NOT_FOUND_ERROR)
			add_history(cmd);
	}
	printf("Have a nice day! Dear.\n");
	return EXIT_SUCCESS;
}

int main(void)
{

	wd = "/";
	create_ctx(200000, shell, NULL, 0, "shell");
	irq_enable();
	start_sched();
	umount();
	return EXIT_SUCCESS;
}
