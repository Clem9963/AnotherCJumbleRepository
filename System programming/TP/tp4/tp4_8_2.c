#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int status = 0;
	pid_t pid = 0;
	char *argv[] = {NULL};
	
	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "Erreur lors de la création du nouveau processus\n");
	}
	else if (pid == 0)
	{
		execv("./retourne3", argv);
	}
	else
	{
		wait(&status);
		printf("%d\n", WEXITSTATUS(status));
	}

	return EXIT_SUCCESS;
}
