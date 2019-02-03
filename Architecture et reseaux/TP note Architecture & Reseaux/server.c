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
#define BUFFER_SIZE 1024
#define FORMATTING_BUFFER_SIZE 1048
#define TRUE 1
#define FALSE 0

struct Client
{
	char username[16];
	int csock;
	int nb_coups;
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

	if (argc != 4)
	{
		fprintf(stderr, "Arguments fournis incorrects\n");
		fprintf(stderr, "Il faut entrer dans l'ordre k, n et le port\n");
		return EXIT_FAILURE;
	}

	char buffer[BUFFER_SIZE] = "";								// Buffer de 1024 octets pour les données brutes car ajout, par la suite, du nom d'utilisateur
	char formatting_buffer[FORMATTING_BUFFER_SIZE] = "";			// Buffer de 1048 pour le broadcast : message formaté avec nom d'utilisateur
	struct Client clients[MAX_CLIENTS];
	int clients_nb = 0;

	int k = atoi(argv[1]);
	int n = atoi(argv[2]);
	int port = atoi(argv[3]);
	int proposition = 0;

	int server_sock = 0;

	int selector = 0;
	int max_fd = 0;
	fd_set readfs;

	int correct = FALSE;
	int reset = 0;
	char *char_ptr = NULL;

	int i = 0;
	int j = 0;

	if (k < 0 || k > n)
	{
		fprintf(stderr, "Il faut entrer dans l'ordre k, n et le port\n");
		return EXIT_FAILURE;
	}

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
			strcpy(formatting_buffer, "Connexion de ");
			strcat(formatting_buffer, clients[clients_nb-1].username);
			formatting_buffer[BUFFER_SIZE-1] = '\0';
			strcpy(buffer, formatting_buffer);
			printf("%s\n", buffer);
			for (j = 0; j < clients_nb; j++)
			{	
				send_client(clients[j].csock, buffer);
			}
			sprintf(buffer, "Le nombre maximal est %d", n);
			send_client(clients[clients_nb-1].csock, buffer);
		}

		for (i = 0; i < clients_nb; i++)
		{
			if(FD_ISSET(clients[i].csock, &readfs))
			{
				/* Des données sont disponibles sur un des sockets clients */

				if (!recv_client(clients[i].csock, buffer, sizeof(buffer)))	// Si le client se déconnecte
				{
					strcpy(formatting_buffer, "Déconnexion de ");
					strcat(formatting_buffer, clients[i].username);
					formatting_buffer[BUFFER_SIZE-1] = '\0';
					strcpy(buffer, formatting_buffer);
					printf("%s\n", buffer);
					for (j = 0; j < clients_nb; j++)
					{	
						send_client(clients[j].csock, buffer);
					}
					rmv_client(clients, i, &clients_nb, &max_fd, server_sock);
				}
				else	// Ce qui se trouvait dans le buffer du client est tout de même envoyé, d'où le "else"
				{
					clients[i].nb_coups--;
					if (clients[i].nb_coups == 0)				// Si tous les coups sont utilisés
					{
						strcpy(formatting_buffer, clients[i].username);
						strcat(formatting_buffer, " a utilisé tous ses coups");
						formatting_buffer[BUFFER_SIZE-1] = '\0';
						strcpy(buffer, formatting_buffer);
						printf("%s\n", buffer);
						for (j = 0; j < clients_nb; j++)
						{	
							send_client(clients[j].csock, buffer);
						}
					}
					else if (clients[i].nb_coups < 0)
					{
						strcpy(formatting_buffer, "Vous avez utilisé tous vos coups");
						formatting_buffer[BUFFER_SIZE-1] = '\0';
						strcpy(buffer, formatting_buffer);
						printf("(%s) %s\n", clients[i].username, buffer);
						send_client(clients[i].csock, buffer);
					}
					else
					{
						proposition = atoi(buffer);

						if (proposition < k)
						{
							strcpy(formatting_buffer, buffer);
							strcat(formatting_buffer, " ? ");
							strcat(formatting_buffer, "c'est plus");
						}
						else if (proposition > k)
						{
							strcpy(formatting_buffer, buffer);
							strcat(formatting_buffer, " ? ");
							strcat(formatting_buffer, "c'est moins");
						}
						else
						{
							strcpy(formatting_buffer, clients[i].username);
							strcat(formatting_buffer, " a trouvé la réponse ! Félicitations !");

							formatting_buffer[BUFFER_SIZE-1] = '\0';
							strcpy(buffer, formatting_buffer);
							printf("%s\n", buffer);
							for (j = 0; j < clients_nb; j++)
							{	
								send_client(clients[j].csock, buffer);
							}

							for (j = 0; j < clients_nb; j++)
							{
								clients[j].nb_coups = 5;
							}

							correct = FALSE;

							while (!correct)	// Tant que le k entré n'est pas valide
							{
								printf("Veuillez rentrer un nouveau k\n");

								if (fgets(buffer, sizeof(buffer), stdin) == NULL)
								{
									perror("fgets error");
									exit(errno);
								}

								/* Flush du flux stdin */

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
								reset = 0;	// Réinitialisation de reset pour le prochain fgets

								correct = TRUE;

								for (j = 0; j < (int)strlen(buffer); j++)
								{
									correct = correct && buffer[i] >= '0' && buffer[i] <= '9';
								}
								if (correct)
								{
									k = atoi(buffer);
								}

								correct = correct && k >= 0 && k <= n;
							}

							for (j = 0; j < clients_nb; j++)
							{	
								sprintf(formatting_buffer, "Le nombre maximal est %d", n);
							}
						}
						formatting_buffer[BUFFER_SIZE-1] = '\0';
						strcpy(buffer, formatting_buffer);
						printf("%s\n", buffer);
						for (j = 0; j < clients_nb; j++)
						{	
							send_client(clients[j].csock, buffer);
						}
					}
				}
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
	struct Client client = {"", 0, 5};
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
