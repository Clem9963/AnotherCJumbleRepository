#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void recupEntree(char *adresseIPDec, int *nbCidr, char *argv);
void divisionReseau(char *adresse, int k, int nbCidr, int nbBits);
int verification(int nbCidr, int k);

int main(int argc, char *argv[]){
     
     if(argc != 3)
     {
          fprintf(stderr, "Veuillez rentrer une IP/cidr et un entier k\n");
          exit(-1);
     }
     else if(strlen(argv[1]) > 18)
     {
          fprintf(stderr, "adresse ip/cidr trop long ! \n");
          exit(-1);
     }
     

     char adresseIPDec[15];
     int nbCidr = 0;
     int k = atoi(argv[2]);
     int nbBits = 0;
          
     //recuperation de l'adresse ip et du nombre cidr
     recupEntree(adresseIPDec, &nbCidr, argv[1]);
     
     nbBits = verification(nbCidr, k);

     //affiche les sous reseaux
     divisionReseau(adresseIPDec, k, nbCidr, nbBits);

     return 0;
}

void recupEntree(char *adresseIPDec, int *nbCidr, char *argv){
     int i = 0, j = 0;
     char nb[3];
     while(argv[i] != '/')
     {
          i++;
     }
     for(j = 0; j < i; j++)
     {
          adresseIPDec[j] = argv[j];
     }

     *nbCidr = (argv[i + 1] - '0') * 10 + (argv[i + 2] - '0');
     
     if(*nbCidr > 32)
     {
   	 	fprintf(stderr, "cidr trop grand !\n");
   	 	exit(-1);
   	 }
}

int verification(int nbCidr, int k){
	int i;
	int nb = 0;
	
	nb = 32 - nbCidr;
	
	for (i = 1; i < nb; i++)
	{	
		if(pow(2, i) >= k)
			return i;
	}
	
	fprintf(stderr, "Impossible de diviser en autant de sous reseau\n");
	exit(-1);
}

void divisionReseau(char *adresse, int k, int nbCidr, int nbBits){
     int i, j, indice = 0, compteur = 0;
     int nbBitsRestant = 32 - (nbCidr + nbBits);
     char masque[32];
     char hoteDec[15];
     
     if (nbBitsRestant < 0 || pow(2, nbBitsRestant) < k)
     {
     	fprintf(stderr, "impossible de diviser !\n");
     }
     else
     {
		 printf("bits restants : %d\n", nbBitsRestant);
		 
		 //creation du masque
		 for (i = 0; i < 32; i++)
		 {
		 	if(i == 8 || i == 16 || i == 24)
		 		masque[i] = '.';
		 	else if(i < (32 - nbBitsRestant))
		 		masque[i] = '1';
		 	else
		 		masque[i] = '0';
		 
		 }

		 //recupere l'indice du troisieme point dans l'adresse


		 for (i = 0; i < k; i++)
		 {
		       printf("\n %i sous reseau : ", i + 1);
		       for(j = 0; j < indice; j++)
		            printf("%c", adresse[j]);
		       printf("%d/%d", (int)pow(2, nbBitsRestant) * i, (int)nbCidr + nbBits);

		 }
		 printf("\n\n");
     }
}
