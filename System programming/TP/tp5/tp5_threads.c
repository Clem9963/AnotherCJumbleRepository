#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NB_CHAR_PAGE 5280
#define MIN 65
#define MAX 127

char *generer_page();
char *generer_page2(char lettre);
void liberer_page(char *buffer);
void *afficher_page(void *page);

int main()
{
	char lettre = 65;
	char* page = NULL;
	pthread_t thread_impression = 0;

	while(lettre < 91)
	{
		page = generer_page2(lettre);
		pthread_create(&thread_impression, NULL, afficher_page, page);
		lettre++;
	}
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

char *generer_page2(char lettre)
{
	int i = 0;
	int j = 0;
	char *buffer = malloc(NB_CHAR_PAGE * sizeof(char));

	srand(time(NULL));

	for (i = 0; i < 66; i++)
	{
		for (j = 0; j < 79; j++)
		{
			buffer[i*80 + j] = lettre;
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

void *afficher_page(void *page_src)
{
	char *page = (char *)page_src;
	printf("%s\n", page);
	return NULL;
}
