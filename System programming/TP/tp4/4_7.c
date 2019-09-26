#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *afficher_100_caracteres(void *caractere);

int main()
{
	pthread_t thread1 = 0;
	pthread_t thread2 = 0;
	pthread_t thread3 = 0;
	char point = '.';
	char hashtag = '#';
	char asterisque = '*';

	pthread_create(&thread1, NULL, afficher_100_caracteres, &asterisque);
	pthread_create(&thread2, NULL, afficher_100_caracteres, &point);
	pthread_create(&thread3, NULL, afficher_100_caracteres, &hashtag);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);

	return EXIT_SUCCESS;
}

void *afficher_100_caracteres(void *caractere)
{
	int i = 0;
	for (i = 0 ; i < 100 ; i++)
	{
		printf("%c", *((char *)caractere));
		fflush(stdout);
	}
	pthread_exit(NULL);
}