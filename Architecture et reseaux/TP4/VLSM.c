#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_ADR_CIDR 19
#define TAILLE_ADR 16
#define TAILLE_ADR_BIN 33

#define TRUE 1
#define FALSE 0

/* Manipulations sur les tableaux */
void trierTableau(int *tab, int taille);
void afficheTab(int *tab, int taille);

/* Manipulations sur les adresses IP */
void extraire_adresse(char *adresse_cidr, char *adresse_dest);
void dec_en_bin(char *adresse_dec, char *adresse_bin);
void bin_en_dec(char *adresse_bin, char *adresse_dec);

/* Calculs annexes */
int calcule_nombre_bits(int nb_hotes);

int main(int argc, char *argv[])
{
	/* Le programme prend un réseau en notation CIDR ainsi que la taille des sous-réseaux demandés. */
	char adr_r_principal[TAILLE_ADR_CIDR];
	char adr_r_principal_sans_cidr[TAILLE_ADR];
	char adr_r_principal_bin[TAILLE_ADR_BIN];
	char temp[TAILLE_ADR_BIN];
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

	strcpy(adr_r_principal, argv[1]);

	trierTableau(tailles_reseaux, nb_sous_reseaux);
	afficheTab(tailles_reseaux, nb_sous_reseaux);
	extraire_adresse(adr_r_principal, adr_r_principal_sans_cidr);
	dec_en_bin(adr_r_principal_sans_cidr, adr_r_principal_bin);
	printf("%s\n", adr_r_principal_sans_cidr);
	printf("%s\n", adr_r_principal_bin);

	printf("%d\n", calcule_nombre_bits(63));

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

void extraire_adresse(char *adresse_cidr, char *adresse_dest)
{
	int i = 0;
	for (i = 0; (i < (int)strlen(adresse_cidr) + 1) && adresse_cidr[i] != '/' ; i++)
	{
		adresse_dest[i] = adresse_cidr[i];
	}
	adresse_dest[i] = '\0';
}

void dec_en_bin(char *adresse_dec, char *adresse_bin)
{
	char adresse_dec_copie[TAILLE_ADR];
	char *token = NULL;
	int nb_dec = 0;
	int nb_bin = 0;
	int num_nb = 0;
	int i = 0;

	strcpy(adresse_dec_copie, adresse_dec);

	for (i = 0; i < TAILLE_ADR_BIN; i++)
	{
		adresse_bin[i] = '0';
	}
	
	/* Récupération du premier token */
	token = strtok(adresse_dec_copie, ".");
	
	/* Parcours des autres */
	while(token != NULL)
	{
		i = 0;
		nb_dec = atoi(token);
		nb_bin = 0;
		while (nb_dec > 0)
		{
			nb_bin += (nb_dec % 2) * pow(10, i);
			i++;
			nb_dec /= 2;
		}

		if (nb_bin < 10000000)
		{
			nb_bin += 10000000;
			sprintf(adresse_bin + 8 * num_nb, "%d", nb_bin);
			*(adresse_bin + 8 * num_nb) = '0';
		}
		else
		{
			sprintf(adresse_bin + 8 * num_nb, "%d", nb_bin);
		}
	
		token = strtok(NULL, ".");
		num_nb++;
	} 
}

void bin_en_dec(char *adresse_bin, char *adresse_dec)
{
	int i = 0;
	int nb_temp[4] = {0};

	for (i = 0; i < TAILLE_ADR_BIN; i++)
	{
		nb_temp[i/8] += pow(2, 7 - i % 8) * (adresse_bin[i] - '0');
	}

	sprintf(adresse_dec, "%d.%d.%d.%d", nb_temp[0], nb_temp[1], nb_temp[2], nb_temp[3]);
}

int calcule_nombre_bits(int nb_hotes)
{
	nb_hotes += 2;
	int i = 0;

	for (i = 0; i < 32 && pow(2, i) < nb_hotes; i++)
	{
		continue;
	}

	return i;
}