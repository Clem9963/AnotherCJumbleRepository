#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define h_addr h_addr_list[0]
#define SOCKET_ERROR -1 /* code d'erreur des sockets  */
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

/* Fonction d'initialisation */
int connect_socket(char* address, int port);

/* Fonctions liées à l'envoi et à la réception */
int recv_server(int sock, char *buffer, size_t buffer_size);
int send_server(int sock, char *buffer);

int main(int argc, char *argv[])
{
	/* La fonction main attend 3 paramètres :
	-> Le pseudo de l'utilisateur
	-> L'adresse IP du serveur
	-> Le port du serveur */

	char buffer[BUFFER_SIZE] = "";		// Buffer de 1024 octets pour l'envoi et la réception

	char *username = NULL;
	char *address = NULL;
	int port = 0;
	int i, verif = 0;

	int server_sock = 0;
	int selector = 0;
	fd_set readfs;

	int reset = 0;
	char *char_ptr = NULL;

	//si le nombre d'arguments est insuffisant
	if (argc != 4)
	{
		fprintf(stderr, "\nArguments fournis incorrects\n");
		printf("L'execution est de la forme ./client pseudo adresse du serveur port\n\n");
		return EXIT_FAILURE;
	}
	//si le nombe d'argument est correct
	else
	{	
		printf("\nPour jouer, envoyer des messages de la forme suivante : (un chiffre)\n\n");
	}

	//recuperation des parametres
	username = argv[1];
	if (strlen(username) > 15)
	{
		username[15] = '\0';	// Pas de pseudo trop long !
	}
	address = argv[2];
	port = atoi(argv[3]);

	//connexion au serveur
	server_sock = connect_socket(address, port);

	//envoi du pseudo au serveur
	send_server(server_sock, username);

	while(TRUE)
	{
		FD_ZERO(&readfs);
		FD_SET(server_sock, &readfs);
		FD_SET(STDIN_FILENO, &readfs);

		if((selector = select(server_sock + 1, &readfs, NULL, NULL, NULL)) < 0)
		{
			perror("select error");
			exit(errno);
		}
		
		if(FD_ISSET(server_sock, &readfs))
		{
			//Probleme lors de la reception du message du serveur
			if (!recv_server(server_sock, buffer, sizeof(buffer)))
			{
				printf("\nLa connexion au serveur a été interrompue\n\n");
				close(server_sock);
				exit(EXIT_SUCCESS);
			}
			//si la reception a fonctionnée on affiche le message et le demande du chiffre
			else
			{
				printf("Serveur : %s\n", buffer);
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

			//permet de flush le stdin
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
			
			//verification du chiffre rentré pour verifier que c'est bien un chiffre
			i = 0;
			while(buffer[i] != '\0')
			{
				if(buffer[i] < '0' || buffer[i] > '9')
				{
					verif = 1;
					break;
				}	
				i++;
			}

			if(verif == 1){
				printf("\n Veuillez rentrer un chiffre ! \n");
				verif = 0;
			}
			else
				send_server(server_sock, buffer);
		}
	}

	return EXIT_SUCCESS;
}

int connect_socket(char* address, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct hostent* hostinfo;
	struct sockaddr_in sin; /* structure qui possède toutes les infos pour le socket */

	if(sock == SOCKET_ERROR)
	{
		perror("socket error");
		exit(errno);
	}

	hostinfo = gethostbyname(address); /* infos du serveur */

	if (hostinfo == NULL) /* gethostbyname n'a pas trouvé le serveur */
	{
		herror("gethostbyname");
        exit(errno);
	}

	printf("Connexion à %u.%u.%u.%u\n", (hostinfo->h_addr[0]+256)%256, (hostinfo->h_addr[1]+256)%256, (hostinfo->h_addr[2]+256)%256, (hostinfo->h_addr[3]+256)%256);

	sin.sin_addr = *(struct in_addr*) hostinfo->h_addr; /* on spécifie l'adresse */
	sin.sin_port = htons(port); /* le port */
	sin.sin_family = AF_INET; /* et le protocole (AF_INET pour IP) */

	if (connect(sock, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == SOCKET_ERROR) /* demande de connexion */
	{
		perror("connect error");
		exit(errno);
	}

	return sock;
}

//recoit les messages du serveur
int recv_server(int sock, char *buffer, size_t buffer_size)
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

//envoi les messages au serveur
int send_server(int sock, char *buffer)
{
	if (send(sock, buffer, strlen(buffer)+1, 0) == SOCKET_ERROR) 	// Le +1 représente le caractère nul
	{
		perror("send error");
		exit(errno);
	}

	return EXIT_SUCCESS;
}
