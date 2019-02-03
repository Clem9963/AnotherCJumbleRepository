#include "test.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

Arbre* initArbre()
{
	void creerRelation(Noeud *noeud)
	{
		/* Permet de créer une feuille représentant la relation {1} */
		noeud->etiquette = '1';
		noeud->numero = 0;
		noeud->filsG = NULL;
		noeud->filsD = NULL;
	}

	Arbre *arbre = calloc(1, sizeof(Arbre));
	arbre->racine = calloc(1, sizeof(Noeud));
	creerRelation(arbre->racine);
	arbre->nbFeuilles = 1;
	return arbre;
}

Arbre* unionArbres(Arbre *arbre1, Arbre *arbre2, char operation)
{
	if (operation != '+' && operation != '*')
	{
		fprintf(stderr, "Erreur unionArbres : paramètres invalides\n");
	}

	Noeud *nvRacine = calloc(1, sizeof(Noeud));
	nvRacine->etiquette = operation;
	nvRacine->numero = 0;
	nvRacine->filsG = arbre1->racine;
	nvRacine->filsD = arbre2->racine;
	arbre1->racine = nvRacine;
	arbre1->nbFeuilles = arbre1->nbFeuilles + arbre2->nbFeuilles;
	free(arbre2);
	return arbre1;
}

void majArbre(Arbre *arbre)
{
	void majNumeros(Noeud *racine, int *numeroCourant)
	{
		if (racine->etiquette == '1')
		{
			racine->numero = *(numeroCourant);
			*numeroCourant = *numeroCourant + 1;
		}
		else	// Le noeud a forcément deux fils par construction
		{
			majNumeros(racine->filsG, numeroCourant);
			majNumeros(racine->filsD, numeroCourant);
		}
	}

	int numero = 1;
	majNumeros(arbre->racine, &numero);
}

void arbreEnMatrice(int taille, Arbre *arbre, int **matrice)
{
	int i = 0;

	int* NoeudEnMatrice(Noeud *noeud, int **matrice)
	{
		int i = 0;
		int j = 0;
		int k = 0;
		int *tabResult = NULL;
		int *tabFilsG = NULL;
		int *tabFilsD = NULL;

		if (noeud->etiquette == '+')
		{	
			tabFilsG = NoeudEnMatrice(noeud->filsG, matrice);
			tabFilsD = NoeudEnMatrice(noeud->filsD, matrice);
			tabResult = calloc(tabFilsG[0] + tabFilsD[0] - 1, sizeof(int));	// -1 car on a besoin d'une case pour connaître la taille et non deux
			tabResult[0] = tabFilsG[0] + tabFilsD[0] - 1;
			for (i = 1; i < tabFilsG[0]; i++)
			{
				tabResult[i] = tabFilsG[i];
			}
			for (j = 1; j < tabFilsD[0]; i++, j++)
			{
				tabResult[i] = tabFilsD[j];
			}
			free(tabFilsG);
			free(tabFilsD);
			return tabResult;
		}
		if (noeud->etiquette == '*')
		{	
			tabFilsG = NoeudEnMatrice(noeud->filsG, matrice);
			tabFilsD = NoeudEnMatrice(noeud->filsD, matrice);
			tabResult = calloc(tabFilsG[0] + tabFilsD[0] - 1, sizeof(int));
			tabResult[0] = tabFilsG[0] + tabFilsD[0] - 1;
			for (i = 1; i < tabFilsG[0]; i++)
			{
				tabResult[i] = tabFilsG[i];
				for (k = 1; k < tabFilsD[0]; k++)
				{
					matrice[tabFilsG[i]][tabFilsD[k]] = 1;
				}
			}
			for (j = 1; j < tabFilsD[0]; i++, j++)
			{
				tabResult[i] = tabFilsD[j];
				for (k = 1; k < tabFilsG[0]; k++)
				{
					matrice[tabFilsD[j]][tabFilsG[k]] = 1;
				}
			}
			free(tabFilsG);
			free(tabFilsD);
			return tabResult;
		}
		else
		{
			tabResult = calloc(2, sizeof(int));
			tabResult[0] = 2;
			tabResult[1] = noeud->numero;
			return tabResult;
		}
	}
	
	majArbre(arbre);
	matrice[0][0] = 0;		// Initialisation du coin supérieur gauche même ci ce dernier ne sera pas utilisé
	for (i = 1; i < taille; i++)
	{
		matrice[0][i] = i;	// Etiquetage des colonnes pour l'affichage
		matrice[i][0] = i;	// Etiquetage des lignes pour l'affichage
	}
	free(NoeudEnMatrice(arbre->racine, matrice));
}

void arbreEnListe(int taille, Arbre *arbre, Element *liste[taille])
{
	int* NoeudEnListe(int taille, Noeud *noeud, Element *liste[taille])
	{
		int i = 0;
		int j = 0;
		int k = 0;
		int *tabResult = NULL;
		int *tabFilsG = NULL;
		int *tabFilsD = NULL;

		if (noeud->etiquette == '+')
		{	
			tabFilsG = NoeudEnListe(taille, noeud->filsG, liste);
			tabFilsD = NoeudEnListe(taille, noeud->filsD, liste);
			tabResult = calloc(tabFilsG[0] + tabFilsD[0] - 1, sizeof(int));	// -1 car on a besoin d'une case pour connaître la taille et non deux
			tabResult[0] = tabFilsG[0] + tabFilsD[0] - 1;
			for (i = 1; i < tabFilsG[0]; i++)
			{
				tabResult[i] = tabFilsG[i];
			}
			for (j = 1; j < tabFilsD[0]; i++, j++)
			{
				tabResult[i] = tabFilsD[j];
			}
			free(tabFilsG);
			free(tabFilsD);
			return tabResult;
		}
		if (noeud->etiquette == '*')
		{	
			tabFilsG = NoeudEnListe(taille, noeud->filsG, liste);
			tabFilsD = NoeudEnListe(taille, noeud->filsD, liste);
			tabResult = calloc(tabFilsG[0] + tabFilsD[0] - 1, sizeof(int));
			tabResult[0] = tabFilsG[0] + tabFilsD[0] - 1;
			for (i = 1; i < tabFilsG[0]; i++)
			{
				tabResult[i] = tabFilsG[i];
				for (k = 1; k < tabFilsD[0]; k++)
				{
					definirL(taille, liste, tabFilsG[i], tabFilsD[k], 1);
				}
			}
			for (j = 1; j < tabFilsD[0]; i++, j++)
			{
				tabResult[i] = tabFilsD[j];
				for (k = 1; k < tabFilsG[0]; k++)
				{
					definirL(taille, liste, tabFilsD[j], tabFilsG[k], 1);
				}
			}
			free(tabFilsG);
			free(tabFilsD);
			return tabResult;
		}
		else
		{
			tabResult = calloc(2, sizeof(int));
			tabResult[0] = 2;
			tabResult[1] = noeud->numero;
			return tabResult;
		}
	}
	
	majArbre(arbre);
	libererL(taille, liste);
	free(NoeudEnListe(taille, arbre->racine, liste));
}

void libererArbre(Arbre *arbre)
{
	void libererNoeud(Noeud *noeud)
	{
		if (noeud != NULL)
		{
			libererNoeud(noeud->filsG);
			libererNoeud(noeud->filsD);
			free(noeud);
		}
	}

	libererNoeud(arbre->racine);
	free(arbre);
}
