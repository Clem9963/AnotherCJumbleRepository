#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NB_CHAR_PAGE 5280
#define MIN 65
#define MAX 127

char *generer_page();
void liberer_page(char *buffer);

int main()
{
	char* page = NULL;
	int bouts_tube[2];
	pid_t impression = 0;
	pipe(bouts_tube);

	impression = fork();
	
	if (impression == -1)
	{
		fprintf(stderr, "Erreur lors de la création du nouveau processus pour l'impression\n");
	}
	else if (impression == 0)
	{
		// Processus fils : impression
		while(TRUE)
		{
			sleep(2);
			page = malloc(NB_CHAR_PAGE * sizeof(char));
			read(bouts_tube[0], page, NB_CHAR_PAGE * sizeof(char));
		}
		close(bouts_tube[0]);
	}
	else
	{
		// Processus père : réseau
		while(TRUE)
		{
			char* page = generer_page();
			printf("%s\n", page);
			write(bouts_tube[1], page, NB_CHAR_PAGE * sizeof(char));
		}
		close(bouts_tube[1]);
	}

	return EXIT_SUCCESS;
}

char *generer_page()
{
	int i = 0;
	int j = 0;
	char *buffer = malloc(NB_CHAR_PAGE * sizeof(char));

	srand(time(NULL));

	for (i = 0; i < 66; i++)
	{
		for (j = 0; j < 79; j++)
		{
			buffer[i*80 + j] = (rand() % (MAX - MIN + 1)) + MIN;
		}
		buffer[i*80 + j] = '\n';
	}
	buffer[NB_CHAR_PAGE - 1] = '\0';

	return buffer;
}

void liberer_page(char *buffer)
{
	free(buffer);
}