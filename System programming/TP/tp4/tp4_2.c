#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int x = 0;
	pid_t pid = 0;
	
	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "Erreur lors de la création du nouveau processus\n");
	}
	else if (pid == 0)
	{
		printf("En Chine, quand t'as pas de bol, t'as pas de riz\n");
		x = 181;
		printf("Je regrette ce bon vieux Dichi\n");
	}
	else
	{
		sleep(2);
		printf("x vaut maintenant : %d dans le processus père\n", x);
	}

	return EXIT_SUCCESS;
}

// La valeur de x n'est pas modifiée dans le processus père