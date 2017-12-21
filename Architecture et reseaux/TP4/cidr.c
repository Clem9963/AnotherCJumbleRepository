#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertir(char *adresseIp, char *binEntier);
void  binaire(char *chiffre, char *temp);
void ajoute(char *binEntier, char *temp);
void cidr(char * binEntier);

int main(int argc, char *argv[]){
     
     if(argc != 2)
     {
          printf("Veuillez rentrer un masque de sous reseau !!\n");
          exit(-1);
     }
     else if(strlen(argv[1]) > 16)
     {
          printf("adresse ip trop longue ! \n");
          exit(-1);
     }
     
     char binEntier[35] = {0};
     
     convertir(argv[1], binEntier);
     
     printf("\n L'adresse ip en binaire est : \n %s\n\n", binEntier);
     
     cidr(binEntier);
     

     return 0;
}


void convertir(char *adresseIp, char *binEntier){
     int i, j = 0, k = 0;
     char chiffre[4];
     char temp[8];
     int compteurPoint = 0;
     
     for(i = 0; i < 16; i++)
     {
          if (adresseIp[i] == '\0')
          {
          	   chiffre[3] = '\0';
               binaire(chiffre, temp);
               ajoute(binEntier, temp);
               binEntier[35] = '\0';
               break;
          }
          if(adresseIp[i] != '.')
          {
               chiffre[j] = adresseIp[i];
               j++;
          }
        
          else
          {
          	   chiffre[3] = '\0';
               binaire(chiffre, temp);
               ajoute(binEntier, temp);
               j = 0;
               chiffre[0] = ' ';
               chiffre[1] = ' ';
               chiffre[2] = ' ';
               compteurPoint++;
          }
     }
}

void  binaire(char *chiffre, char *temp){
     int ch = atoi(chiffre);
     int i, n = 0;
     int puissance = 0;
     char nb[] = "00000000";
     
     for(i = 7; i >= 0 ;i--)
     {
          nb[i] = (ch % 2) + '0';
          ch = ch / 2;
     }
     strcpy(temp, nb);
}

void ajoute(char *binEntier, char *temp){
     strcat(binEntier, temp);
     strcat(binEntier, ".");

}
void cidr(char * binEntier){
     int i, compteur = 0;
     
     for (i = 0; i < strlen(binEntier); i++)
     {
          if(binEntier[i] == '1')
               compteur++;
     }
     printf("\nNotation cidr : /%d\n\n", compteur);
}






















