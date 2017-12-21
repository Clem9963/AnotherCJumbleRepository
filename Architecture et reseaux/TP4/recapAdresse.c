#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void convertir(char *adresseIp, char *binEntier);
void  binaire(char *chiffre, char *temp);
void ajoute(char *binEntier, char *temp);
void cidr(char * binEntier);
void sToCidr(int *nb, char *argv);
void masqueSousReseau(int nb, char *masque);
void adresses(char *adresseBin, char *masque, char *adrSousReseau, char *adrHote);
void adrBinToDec(char *adresse, char *adrBinDec);
void afficheTable(char *adresseIP, char *adresseBin, char *masqueBinDec, char *masque, char *adrSousReseauBinDec, char *adrSousReseau, char *adrHoteBinDec, char *adrHote);


int main(int argc, char *argv[]){
     
     if(argc != 3)
     {
          printf("Veuillez rentrer une IP et une notation cidr\n");
          exit(-1);
     }
     else if(strlen(argv[1]) > 16)
     {
          printf("adresse ip trop longue ! \n");
          exit(-1);
     }
     else if(strlen(argv[2]) < 2)
     {
          printf("Ce n'est pas une notation cidr !!\n");
     }
     

     char adresseIP[16];
     strcpy(adresseIP, argv[1]);
     //adresses en binaires
     char adresseBin[35] = {0};
     char masque[35] = {0};
     char adrSousReseau[35] = {0};
     char adrHote[35] = {0};

     //adresses en decimale
     char masqueBinDec[15] = {0};
     char adrSousReseauBinDec[15] = {0};
     char adrHoteBinDec[15] = {0};

     int nbCidr = 0;
          
     //determination de l'adresse IP en binaire
     convertir(argv[1], adresseBin);
         
     //recuperation du cidr en int
     sToCidr(&nbCidr, argv[2]);

     if (nbCidr < 0 || nbCidr > 32)
     {
          fprintf(stderr, "Nombre cidr incorrect !!\n");
          exit(-1);
     }

     //recuperation du masque
     masqueSousReseau(nbCidr, masque);

     //recuperation de l'adresse de sous reseau et de l'adresse de l'hote
     adresses(adresseBin, masque, adrSousReseau, adrHote);

     //recuperation des adresses decimales
     adrBinToDec(masque, masqueBinDec);
     adrBinToDec(adrSousReseau, adrSousReseauBinDec);
     adrBinToDec(adrHote, adrHoteBinDec);

     afficheTable(adresseIP, adresseBin, masqueBinDec, masque, adrSousReseauBinDec, adrSousReseau, adrHoteBinDec, adrHote);

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
               binaire(chiffre, temp);
               ajoute(binEntier, temp);
               j = 0;
               strcpy(chiffre, " ");
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

void sToCidr(int *nb, char *argv){
     int i;
     for (i = 1; i < strlen(argv); i++)
     {
          *nb = *nb + (argv[i] - '0')* pow(10, strlen(argv) - 1 - i);
     }
}

void masqueSousReseau(int nb, char *masque){
     int i, j = 1;
     for (i = 0; i < 35; i++)
     {    
          if(i == 8 *j + j-1){
               masque[i] = '.';
               j++;
          }
          else if (i < nb + j- 1 )
               masque[i] = '1';
          else
               masque[i] = '0';
     }
     masque[35] = '\0';
}

void adresses(char *adresseBin, char *masque, char *adrSousReseau, char *adrHote){
     int i = 0;

     while(masque[i] != '0')
     {
          adrSousReseau[i] = adresseBin[i];
          //pour gerer les points
          if (masque[i] == '.')
               adrHote[i] = '.';
          else
               adrHote[i] = '0';
          i++;
     }

     while(adresseBin[i] != '\0')
     {
          //pour gerer les points
          if (adresseBin[i] == '.')
               adrSousReseau[i] = '.';
          else
               adrSousReseau[i] = '0';
          adrHote[i] = adresseBin[i];
          i++;
     }
     adrSousReseau[i] = '\0';
}

void adrBinToDec(char *adresse, char *adrBinDec){
     int nb = 0, i, j;
     char temp[3];

     for (j = 0; j < 4; j++)
     {
          nb = 0;
          for (i = 0; i < 8; i++)
          {
               nb = nb + (adresse[i + j*8 + j] - '0') * pow(2, 7-i);
          }

          sprintf(temp, "%d", nb);
          strcat(adrBinDec, temp);
          if (j != 3)
               strcat(adrBinDec, ".");
     }
}

void afficheTable(char *adresseIP, char *adresseBin, char *masqueBinDec, char *masque, 
                    char *adrSousReseauBinDec, char *adrSousReseau, char *adrHoteBinDec, 
                         char *adrHote){
     
     int i;
     printf("\n\n");

     for(i = 0; i < 80; i++)
          printf("-");
     printf("\n                  Notation Dec.          Notation binaire\n");
     for(i = 0; i < 80; i++)
          printf("-");
     printf("\n Adresse IPv4\t%s \t%s\n", adresseIP, adresseBin);
     printf(" Adresse IPv4\t%s \t%s\n", masqueBinDec, masque);
     printf(" Adresse IPv4\t%s \t%s\n", adrSousReseauBinDec, adrSousReseau);
     printf(" Adresse IPv4\t%s \t%s\n", adrHoteBinDec, adrHote);
     for(i = 0; i < 80; i++)
          printf("-");
     printf("\n\n");
}

