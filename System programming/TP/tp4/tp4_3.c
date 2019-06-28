#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void afficher_100_caracteres(char caractere);

int main()
{
	pid_t pid1 = 0;
	pid_t pid2 = 0;

	pid1 = fork();
	if (pid1 == -1)
	{
		fprintf(stderr, "Echec lors de la création du processus");
	}
	else if (pid1 == 0)
	{
		afficher_100_caracteres('*');
	}
	else
	{
		pid2 = fork();
		if (pid2 == -1)
		{
			fprintf(stderr, "Echec lors de la création du processus");
		}
		else if (pid2 == 0)
		{
			afficher_100_caracteres('.');
		}
		else
		{
			afficher_100_caracteres('#');
			wait(NULL);
			wait(NULL);
		}
	}

	return EXIT_SUCCESS;
}

void afficher_100_caracteres(char caractere)
{
	int i = 0;
	for (i = 0 ; i < 100 ; i++)
	{
		printf("%c", caractere);
		fflush(stdout);
	}
}