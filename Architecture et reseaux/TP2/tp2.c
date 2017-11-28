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
#define SOCKET_ERROR -1 /* code d'erreur des sockets  */
#define TRUE 1
#define FALSE 0

int connect_socket(char* addresse, int port);
int read_server(int sock, char *buffer);
int write_server(int sock, char *buffer);

int main(int argc, char *argv[])
{
	/* La fonction main attend 3 paramètres :
	-> Le pseudo de l'utilisateur
	-> L'adresse IP du serveur
	-> Le port du serveur */

	char buffer_send[1024] = "__init_buffer_send__";
	char buffer_recv[1024] = "__init_buffer_recv__";
	char *pseudo = NULL;
	char *addresse = NULL;
	int port = 0;
	int sock = 0;
	int ret = 0;
	fd_set readfs;

	if (argc != 4)
	{
		fprintf(stderr, "Arguments fournis incorrects\n");
		return EXIT_FAILURE;
	}

	pseudo = argv[1];
	addresse = argv[2];
	port = atoi(argv[3]);

	sock = connect_socket(addresse, port);

	write_server(sock, pseudo);

	while(TRUE)
	{
		FD_ZERO(&readfs);
		FD_SET(sock, &readfs);
		FD_SET(STDIN_FILENO, &readfs);
		
		if((ret = select(sock + 1, &readfs, NULL, NULL, NULL)) < 0)
		{
			perror("select()");
			exit(errno);
		}

		/* 
		if(ret == 0)
		{
			ici le code si la temporisation (dernier argument) est écoulée (il faut bien évidemment avoir mis quelque chose en dernier argument).
		}	
		*/
		
		if(FD_ISSET(sock, &readfs))
		{
			/* des données sont disponibles sur le socket */
			/* traitement des données */

			read_server(sock, buffer_recv);
			printf("%s\n", buffer_recv);
		}
	}
	
	return EXIT_SUCCESS;
}

int connect_socket(char* addresse, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == SOCKET_ERROR)
	{
		perror("socket()");
		exit(errno);
	}

	struct hostent* hostinfo = gethostbyname(addresse); /* infos du serveur */

	if (hostinfo == NULL) /* gethostbyname n'a pas trouvé le serveur */
	{
		perror("gethostbyname()");
		exit(errno);
	}

	printf("Connexion à %u.%u.%u.%u\n", hostinfo->h_addr[0], hostinfo->h_addr[1], hostinfo->h_addr[2], hostinfo->h_addr[3]);

	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */

	sin.sin_addr = *(struct in_addr*) hostinfo->h_addr; /* on spécifie l'addresse */
	sin.sin_port = htons(port); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (connect(sock, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == SOCKET_ERROR) /* demande de connection */
	{
		perror("connect");
		exit(errno);
	}

	return sock;
}

int read_server(int sock, char *buffer)
{
	if (recv(sock, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
	{
		perror("recv");
		exit(errno);
	}

	return EXIT_SUCCESS;
}

int write_server(int sock, char *buffer)
{
	if (send(sock, buffer, strlen(buffer), 0) == SOCKET_ERROR)
	{
		perror("send");
		exit(errno);
	}

	return EXIT_SUCCESS;
}