#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NB_CHAR_PAGE 5280
#define MIN 65
#define MAX 127

void *generer_page(void *page_src);
void liberer_page(char *buffer);
void *afficher_page(void *page);

static pthread_mutex_t mutex_reseau = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_impression = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	char page[NB_CHAR_PAGE] = "";


	pthread_t thread_reseau = 0;
	pthread_t thread_impression = 0;

	pthread_create(&thread_impression, NULL, afficher_page, page);
	pthread_create(&thread_reseau, NULL, generer_page, page);

	if (pthread_join(thread_reseau, NULL))
	{
		perror("pthread_join");
		return EXIT_FAILURE;
    }
    if (pthread_join(thread_impression, NULL))
    {
		perror("pthread_join");
		return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void *generer_page(void *page_src)
{
	char lettre = 65;
	while (lettre < 65+26)
	{
		while(pthread_mutex_lock(&mutex_reseau))
		{
			continue;
		}
		int i = 0;
		int j = 0;
		char *page = (char *)page_src;

		srand(time(NULL));

		for (i = 0; i < 66; i++)
		{
			for (j = 0; j < 79; j++)
			{
				// page[i*80 + j] = (rand() % (MAX - MIN + 1)) + MIN;
				page[i*80 + j] = lettre;
			}
			page[i*80 + j] = '\n';
		}
		page[NB_CHAR_PAGE - 1] = '\0';
		lettre++;
		pthread_mutex_unlock(&mutex_impression);
	}
	return NULL;
}

void liberer_page(char *buffer)
{
	free(buffer);
}

void *afficher_page(void *page_src)
{
	while (TRUE)
	{
		while(pthread_mutex_lock(&mutex_impression))
		{
			continue;
		}
		char *page = (char *)page_src;
		printf("%s\n", page);
		pthread_mutex_unlock(&mutex_reseau);
	}
	return NULL;
}
