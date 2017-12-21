#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trierTableau(int *tab, int taille);
void afficheTab(int *tab, int taille);
void convertir(char *adresseIp, char *binEntier);
void binaire(char *chiffre, char *temp);
void ajoute(char *binEntier, char *temp);

int main(int argc, char *argv[])
{
	/* Le programme prend un réseau en notation CIDR ainsi que la taille des sous-réseaux demandés. */
	char adresse_reseau_principal[19];
	int nb_sous_reseaux;
	int *tailles_reseaux;
	int i = 0;

	/* Test des inputs */
	if (argc < 3)
	{
		fprintf(stderr, "Nombre d'arguments incorrect\n");
		exit(EXIT_FAILURE);
	}
	if (strlen(argv[1]) > 18)
	{
		fprintf(stderr, "Adresse fournie incorrecte\n");
		exit(EXIT_FAILURE);
	}

	/* Initialisation */
	nb_sous_reseaux = argc - 2;
	tailles_reseaux = malloc(nb_sous_reseaux * sizeof(int));

	for (i = 0; i < nb_sous_reseaux; i++)
	{
		tailles_reseaux[i] = atoi(argv[i+2]);
	}

	strcpy(adresse_reseau_principal, argv[1]);
	
	char adresse_reseau_principal_sans_cidr[16] = "192.168.1.0";
	char adresse_reseau_principal_bin[36] = "";


	convertir(adresse_reseau_principal_sans_cidr, adresse_reseau_principal_bin);
	printf("%s\n", adresse_reseau_principal_bin);

	return EXIT_SUCCESS;
}

void trierTableau(int *tab, int taille)
{
	int i = 0;
	int j = 0;
	int stockage;

	for (i = 0; i < taille - 1; i++)
	{
		for (j = i; j < taille; j++)
		{
			if(tab[i] < tab[j])
			{
				stockage = tab[i];
				tab[i] = tab[j];
				tab[j] = stockage;
			}
		}
	}
}

void afficheTab(int *tab, int taille)
{
	int i = 0;

	for (i = 0; i < taille; i++)
	{	
		printf("%d\n",tab[i]);	
	}
}

void convertir(char *adresseIp, char *binEntier)
{
	int i, j = 0;
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

void binaire(char *chiffre, char *temp){
	int ch = atoi(chiffre);
	int i = 0;
	char nb[] = "00000000";
	
	for(i = 7; i >= 0 ;i--)
	{
		nb[i] = (ch % 2) + '0';
		ch = ch / 2;
	}
	strcpy(temp, nb);
}

void ajoute(char *binEntier, char *temp)
{
	strcat(binEntier, temp);
	strcat(binEntier, ".");
}