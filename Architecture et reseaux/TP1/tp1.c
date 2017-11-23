#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#define h_addr h_addr_list[0]
#define PORT 41847 /* port arbitraire */
#define BUFFSIZE 100 /* taille des buffers envoyés */
#define SOCKET_ERROR -1 /* code d'erreur des sockets  */

void serveur();
void client();

int main()
{
	client();
	return EXIT_SUCCESS;
}

void serveur()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in csin; int csock = 0; socklen_t sinsize = 0; /* socket client */
	char buffer[1024] = {0};
	
	if(sock == SOCKET_ERROR)
	{
		perror("socket()");
		exit(errno);
	}

	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* on accepte toute adresse */
	sin.sin_port = htons(PORT); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) == SOCKET_ERROR) /* on lie le socket à sin */
	{
		perror("bind()");
		exit(errno);
	}
	listen(sock, 1); /* notre socket est prêt à écouter une connection */


	csock = accept(sock, (struct sockaddr *)&csin, &sinsize); /* accepter un client */
	recv(csock, buffer, sizeof(buffer), 0);

	printf("%s\n", buffer);

	close(csock);
	close(sock);
}

void client()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	char buffer[1024] = "Dietetik !";


	if(sock == SOCKET_ERROR)
	{
		perror("socket()");
		exit(errno);
	}

	struct hostent* hostinfo = gethostbyname("169.254.8.243"); /* infos du serveur */
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
		perror("connect");
		exit(errno);
	}

	if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
	{
		perror("send");
		exit(errno);
	}
	if (close(sock) == SOCKET_ERROR)
	{
		perror("close");
		exit(errno);
	}
}
