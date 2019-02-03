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
#define NB_CHAR_PAGE 5280

#define TRUE 1
#define FALSE 0

int init_connexion(char *adresse_serveur);
void *generer_page(void *page_src);

int main()
{
	int sock;
	char page[NB_CHAR_PAGE] = "";


	sock = init_connexion("localhost");

	generer_page(page);
	if (send(sock, page, strlen(page), 0) == SOCKET_ERROR)
	{
		perror("send error");
		exit(errno);
	}

    return EXIT_SUCCESS;
}

int init_connexion(char *adresse_serveur)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == SOCKET_ERROR)
	{
		perror("socket error");
		exit(errno);
	}

	struct hostent* hostinfo = gethostbyname(adresse_serveur); /* infos du serveur */
	if (hostinfo == NULL) /* gethostbyname n'a pas trouvé le serveur */
	{
		perror("gethostbyname");
		exit(errno);
	}
	printf("Connexion à %u.%u.%u.%u\n", hostinfo->h_addr[0], hostinfo->h_addr[1], hostinfo->h_addr[2], hostinfo->h_addr[3]);

	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */

	sin.sin_addr = *(struct in_addr*) hostinfo->h_addr; /* on spécifie l'addresse */
	sin.sin_port = htons(PORT); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (connect(sock, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == SOCKET_ERROR) /* demande de connection */
	{
		perror("connect error");
		exit(errno);
	}

	return sock;
}

void *generer_page(void *page_src)
{
	char lettre = 65;
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
	return NULL;
}