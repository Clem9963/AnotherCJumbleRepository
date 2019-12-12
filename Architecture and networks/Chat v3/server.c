#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

unsigned int countWords(const char *array);
char mostFrequentLetter(const char *array);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage : serverv4 port\n");
    exit(1);
  }

  int s_ecoute, scom, lg_app;
  struct sockaddr_in adr;
  struct sockaddr_storage recep;
  char buf[1500], renvoi[1500], host[1024], service[20];

  s_ecoute = socket(AF_INET, SOCK_STREAM, 0);
  printf("creation socket\n");
  adr.sin_family = AF_INET;
  adr.sin_port = htons(atoi(argv[1]));
  adr.sin_addr.s_addr = INADDR_ANY;

  if (bind(s_ecoute, (struct sockaddr *)&adr, sizeof(struct sockaddr_in)) !=
      0) {
    printf("probleme de bind sur v4\n");
    exit(1);
  }
  if (listen(s_ecoute, 5) != 0) {
    printf("pb ecoute\n");
    exit(1);
  }

  printf("en attente de connexion\n");
  while (1) {
    scom = accept(s_ecoute, (struct sockaddr *)&recep, (unsigned int *)&lg_app);

    getnameinfo((struct sockaddr *)&recep, sizeof(recep), host, sizeof(host),
                service, sizeof(service), 0);

    printf("recu venant du port %s\n", service);

    while (1) {
      recv(scom, buf, 2, 0);
      if (buf[0] == '1') {
        recv(scom, buf, sizeof(buf), 0);
        sprintf(renvoi, "%d", countWords(buf));
        send(scom, renvoi, strlen(renvoi), 0);
      } else if (buf[0] == '2') {
        recv(scom, buf, sizeof(buf), 0);
        printf("%d", mostFrequentLetter(buf));
        sprintf(renvoi, "%c", mostFrequentLetter(buf));
        send(scom, renvoi, strlen(renvoi), 0);
      }
    }
    close(scom);
  }
  close(s_ecoute);
}

unsigned int countWords(const char *array) {
  unsigned int count = 0;

  while (*array != '\0') {
    if (*array == ' ') {
      ++count;
    }
    ++array;
  }

  return count + 1;
}

char mostFrequentLetter(const char *array) {
  char letters[26] = {0};
  char max_letter = 0;

  while (*array != '\0') {
    ++(letters[*array - 'a']);
    ++array;
  }

  for (char i = 1; i < 26; ++i) {
    if (letters[(size_t)i] > letters[(size_t)max_letter]) {
      max_letter = i;
    }
  }
  return max_letter + 'a';
}