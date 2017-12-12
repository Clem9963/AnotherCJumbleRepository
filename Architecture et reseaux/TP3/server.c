#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SOCKET_ERROR -1 /* code d'erreur des sockets  */
#define MAX_CLIENTS 10 /* Nombre de clients maximum  */
#define TRUE 1
#define FALSE 0

struct Client
{
	char username[16];
	int csock;
};

/* Fonction d'initialisation*/
int listen_socket(int port);

/* Gestion des clients */
struct Client new_client(int ssock, int *nb_c, int *max_fd);
void rmv_client(struct Client *clients, int i_to_remove, int *nb_c, int *max_fd, int server_sock);

/* Envoi et réception */
int recv_client(int sock, char *buffer, size_t buffer_size);
int send_client(int sock, char *buffer);

/* Fonction annexe */
int max(int a, int b);


int main(int argc, char *argv[])
{
	/* La fonction main attend 1 paramètre :
	-> Le port du serveur */

	char buffer[1000] = "";				// Buffer de 1000 octets pour les données brutes car ajout, par la suite, du nom d'utilisateur
	char buffer_s[1024] = "";			// Buffer de 1024 pour le broadcast : message formaté avec nom d'utilisateur
	struct Client clients[MAX_CLIENTS];
	int clients_nb = 0;

	int port = 0;
	int server_sock = 0;

	int selector = 0;
	int max_fd = 0;
	fd_set readfs;

	int reset = 0;
	char *char_ptr = NULL;

	int i = 0;
	int j = 0;

	if (argc != 2)
	{
		fprintf(stderr, "Arguments fournis incorrects\n");
		return EXIT_FAILURE;
	}

	port = atoi(argv[1]);

	server_sock = listen_socket(port);
	max_fd = server_sock;

	while(TRUE)
	{
		FD_ZERO(&readfs);
		FD_SET(server_sock, &readfs);
		FD_SET(STDIN_FILENO, &readfs);
		for (i = 0; i < clients_nb; i++)
		{
			FD_SET(clients[i].csock, &readfs);
		}

		if((selector = select(max_fd + 1, &readfs, NULL, NULL, NULL)) < 0)
		{
			perror("select error");
			exit(errno);
		}
		
		if(FD_ISSET(server_sock, &readfs))
		{
			/* Des données sont disponibles sur le socket du serveur */

			/* Même si clients_nb et modifié par la fonction, c'est toujours l'ancienne valeur qui est prise en compte lors de l'affectation */
			clients[clients_nb] = new_client(server_sock, &clients_nb, &max_fd);
			buffer_s[0] = '\0';
			strcat(buffer_s, "Connexion de ");
			strcat(buffer_s, clients[clients_nb-1].username);
			printf("%s\n", buffer_s);
			for (j = 0; j < clients_nb; j++)
			{	
				send_client(clients[j].csock, buffer_s);
			}
		}

		for (i = 0; i < clients_nb; i++)
		{
			if(FD_ISSET(clients[i].csock, &readfs))
			{
				/* Des données sont disponibles sur un des sockets clients */

				if (!recv_client(clients[i].csock, buffer, sizeof(buffer)))
				{
					buffer_s[0] = '\0';
					strcat(buffer_s, "Déconnexion de ");
					strcat(buffer_s, clients[i].username);
					printf("%s\n", buffer_s);
					for (j = 0; j < clients_nb; j++)
					{	
						send_client(clients[j].csock, buffer_s);
					}
					rmv_client(clients, i, &clients_nb, &max_fd, server_sock);
				}
				else	// Ce qui se trouvait dans le buffer du client est tout de même envoyé, d'où le "else"
				{
					buffer_s[0] = '\0';
					strcat(buffer_s, clients[i].username);
					strcat(buffer_s, " : ");
					strcat(buffer_s, buffer);
					printf("%s\n", buffer_s);
					for (j = 0; j < clients_nb; j++)
					{	
						send_client(clients[j].csock, buffer_s);
					}
				}
			}
		}

		if(FD_ISSET(STDIN_FILENO, &readfs))
		{
			/* Des données sont disponibles sur l'entrée standard */

			if (fgets(buffer, sizeof(buffer), stdin) == NULL)
			{
				perror("fgets error");
				exit(errno);
			}

			char_ptr = strchr(buffer, '\n');
			if (char_ptr != NULL)
			{
				*char_ptr = '\0';
			}
			else
			{
				while (reset != '\n' && reset != EOF)
				{
					reset = getchar();
				}
			}
			buffer_s[0] = '\0';
			strcat(buffer_s, "SERVEUR : ");
			strcat(buffer_s, buffer);
			printf("%s\n", buffer_s);
			for (i = 0; i < clients_nb; i++)
			{	
				send_client(clients[i].csock, buffer_s);
			}
		}
	}

	return EXIT_SUCCESS;
}

int listen_socket(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */
	memset(&sin, 0, sizeof(sin));
	
	if (sock == SOCKET_ERROR)
	{
		perror("socket error");
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* on accepte toute adresse */
	sin.sin_port = htons(port); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (bind(sock, (struct sockaddr*) &sin, sizeof(sin)) == SOCKET_ERROR) /* on lie le socket à sin */
	{
		perror("bind error");
		exit(errno);
	}

	if (listen(sock, 1) == SOCKET_ERROR) /* notre socket est prêt à écouter une connexion à la fois */
	{
    	perror("listen error");
    	exit(errno);
	}

	return sock;
}

struct Client new_client(int ssock, int *nb_c, int *max_fd)
{
	int csock = 0;
	struct sockaddr_in csin;
	socklen_t sinsize = sizeof(csin); /* socket client */

	char username[16] = "";

	memset(&csin, 0, sizeof(csin));;
	csock = accept(ssock, (struct sockaddr *)&csin, &sinsize); /* accepter un client */
	if (csock == SOCKET_ERROR)
	{
		perror("accept error");
		exit(errno);
	}
	*nb_c += 1;
	*max_fd = max(csock, *max_fd);

	if (recv(csock, username, 16, 0) == SOCKET_ERROR)
	{
		perror("recv error");
		exit(errno);
	}

	username[15] = '\0';	// On s'assure que le username ne fasse pas plus de 16 caractères
	struct Client client = {"", 0};
	client.csock = csock;
	strcpy(client.username, username);

	return client;
}

void rmv_client(struct Client *clients, int i_to_remove, int *nb_c, int *max_fd, int server_sock)
{
	int i = 0;

	close(clients[i_to_remove].csock);
	for (i = i_to_remove; i < *nb_c-1; i++)
	{
		clients[i] = clients[i+1];
	}

	*nb_c -= 1;
	*max_fd = server_sock;
	for (i = 0; i < *nb_c; i++)
	{
		*max_fd = max(*max_fd, clients[i].csock);
	}
}

int recv_client(int sock, char *buffer, size_t buffer_size)
{
	ssize_t recv_outcome = 0;
	recv_outcome = recv(sock, buffer, buffer_size, 0);
	if (recv_outcome == SOCKET_ERROR)
	{
		perror("recv error");
		exit(errno);
	}
	else if (recv_outcome == 0)
	{
		return FALSE;
	}

	return TRUE;
}

int send_client(int sock, char *buffer)
{
	if (send(sock, buffer, strlen(buffer)+1, 0) == SOCKET_ERROR)	// Le +1 représente le caractère nul
	{
		perror("send error");
		exit(errno);
	}

	return TRUE;
}

int max(int a, int b)
{
	if (a >= b)
	{
		return a;
	}
	else
	{
		return b;
	}
}