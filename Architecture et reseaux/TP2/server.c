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

int init_server(int port);
int read_client(int sock, char *buffer, size_t buffer_size);
int write_client(int sock, char *buffer);
int max(int a, int b);


int main(int argc, char const *argv[])
{
	/* La fonction main attend 1 paramètre :
	-> Le port du serveur */

	char buffer_send[1024] = "";
	char buffer_recv[1024] = "";	
	
	char *username = NULL;
	int port = 0;

	int server_sock = 0;
	int client_sock = 0;
	struct sockaddr_in client_sin;
	socklen_t client_sinsize = sizeof(client_sin);

	int selector = 0;
	int fd_max = 0;
	fd_set readfs;

	int bouncer = 0;
	char *char_ptr = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "Arguments fournis incorrects\n");
		return EXIT_FAILURE;
	}

	port = atoi(argv[1]);

	server_sock = init_server(port);

	while(TRUE)
	{
		FD_ZERO(&readfs);
		FD_SET(server_sock, &readfs);
		FD_SET(client_sock, &readfs);
		FD_SET(STDIN_FILENO, &readfs);

		fd_max = max(max(server_sock, client_sock), STDIN_FILENO);
		
		if((selector = select(fd_max + 1, &readfs, NULL, NULL, NULL)) < 0)
		{
			perror("select Error : ");
			exit(errno);
		}

		/*if(selector == 0)
		{
			 // ici le code si la temporisation (dernier argument) est écoulée (il faut bien évidemment avoir mis quelque chose en dernier argument). 
		}*/
		
		
		if(FD_ISSET(server_sock, &readfs))
		{
			/* des données sont disponibles sur le socket du serveur */
			/* traitement des données */

			client_sock = accept(server_sock, (struct sockaddr *)&client_sin, &client_sinsize); /* accepter un client */
		}

		if(FD_ISSET(client_sock, &readfs))
		{
			/* des données sont disponibles sur le socket client */
			/* traitement des données */

			read_client(client_sock, buffer_recv, sizeof(buffer_recv));
			printf("%s\n", buffer_recv);
		}

		if(FD_ISSET(STDIN_FILENO, &readfs))
		{
			/* des données sont disponibles sur l'entrée standard' */
			/* traitement des données */

			if (fgets(buffer_send, sizeof(buffer_send), stdin) == NULL)
			{
				perror("fgets Error : ");
				exit(errno);
			}

			char_ptr = strchr(buffer_send, '\n');
			if (char_ptr != NULL)
			{
				*char_ptr = '\0';
			}
			else
			{
				while (bouncer != '\n' && bouncer != EOF)
				{
					bouncer = getchar();
				}
			}
			write_client(client_sock, buffer_send);
		}
	}

	return EXIT_SUCCESS;
}

int init_server(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */
	
	if(sock == SOCKET_ERROR)
	{
		perror("socket()");
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* on accepte toute adresse */
	sin.sin_port = htons(port); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) == SOCKET_ERROR) /* on lie le socket à sin */
	{
		perror("bind()");
		exit(errno);
	}

	if(listen(sock, 1) == SOCKET_ERROR) /* notre socket est prêt à écouter une connexion */
	{
    	perror("listen()");
    	exit(errno);
	}

	return sock;
}

int read_client(int sock, char *buffer, size_t buffer_size)
{
	if (recv(sock, buffer, buffer_size, 0) == SOCKET_ERROR)
	{
		perror("recv");
		exit(errno);
	}

	return EXIT_SUCCESS;
}

int write_client(int sock, char *buffer)
{
	if (send(sock, buffer, strlen(buffer)+1, 0) == SOCKET_ERROR)	// Le +1 représente le caractère nul
	{
		perror("send");
		exit(errno);
	}

	return EXIT_SUCCESS;
}

int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}