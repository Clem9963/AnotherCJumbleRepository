#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char buffer[200], texte[200];
  int sock;
  int answer = -1;
  struct sockaddr_in addr;
  struct hostent *entree;

  if (argc != 3) {
    printf("usage : clientv4 nom_serveur port\n");
    exit(1);
  }

  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_family = AF_INET;

  entree = (struct hostent *)gethostbyname(argv[1]);

  bcopy((char *)entree->h_addr, (char *)&addr.sin_addr, entree->h_length);
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
    printf("probleme connexion\n");
    exit(1);
  }
  printf("connexion passe\n");

  while (answer) {
    printf("Veuillez selectionner une option svp :\n");
    printf("0 : Quitter\n");
    printf("1 : Calculer le nombre de mots d'une phrase\n");
    printf("2 : Renvoyer la lettre la plus utilisée\n\n");

    scanf("%d%*c", &answer);

    printf("Veuillez entrer une phrase svp : \n");
    fgets(texte, 200, stdin);

    if (answer == 1) {
      strcpy(buffer, "1");
    } else if (answer == 2) {
      strcpy(buffer, "2");
    }

    send(sock, buffer, strlen(buffer) + 1, 0);
    send(sock, texte, strlen(texte) + 1, 0);

    recv(sock, buffer, sizeof(buffer), 0);
    printf("Recu %s\n", buffer);
  }
  close(sock);
}