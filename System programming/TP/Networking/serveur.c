#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define h_addr h_addr_list[0]
#define PORT 41847 /* port arbitraire */
#define SOCKET_ERROR -1 /* code d'erreur des sockets  */

#define TRUE 1
#define FALSE 0
#define NB_CHAR_PAGE 5280
#define MIN 65
#define MAX 127

struct pocket
{
	char *page_src;
	int csock;
};
typedef struct pocket pocket;

void *recup_page(void *informations);
void liberer_page(char *buffer);
void *afficher_page(void *page);
void init_serveur(int *sock, int *csock);

static pthread_mutex_t mutex_reseau = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_impression = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	int nb = 0;

	char page[NB_CHAR_PAGE] = "";
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct pocket reseau = {NULL, 0};

	pthread_t thread_reseau = 0;
	pthread_t thread_impression = 0;

	init_serveur(&sock, &(reseau.csock));

	pthread_create(&thread_impression, NULL, afficher_page, page);
	pthread_create(&thread_reseau, NULL, recup_page, (void *)&reseau);

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

void *recup_page(void *informations)
{
	char *page = ((pocket*)informations)->page_src;
	int csock = ((pocket*)informations)->csock;
	while (TRUE)
	{
		while(pthread_mutex_lock(&mutex_reseau))
		{
			continue;
		}
		if (recv(csock, page, NB_CHAR_PAGE * sizeof(char), 0) == 0)
		{
			close(csock);
			page[0] = '\0';
		}
		else
		{
			pthread_mutex_unlock(&mutex_impression);
		}
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

void init_serveur(int *sock, int *csock)
{
	struct sockaddr_in csin;
	socklen_t sinsize = sizeof(csin); /* socket client */
	
	if(*sock == SOCKET_ERROR)
	{
		perror("socket error");
		exit(errno);
	}

	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* on accepte toute adresse */
	sin.sin_port = htons(PORT); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (bind(*sock, (struct sockaddr*) &sin, sizeof(sin)) == SOCKET_ERROR) /* on lie le socket à sin */
	{
		perror("bind error");
		exit(errno);
	}
	listen(*sock, 1); /* notre socket est prêt à écouter une connection */


	*csock = accept(*sock, (struct sockaddr *)&csin, &sinsize); /* accepter un client */
}