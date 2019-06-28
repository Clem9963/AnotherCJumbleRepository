#include "test.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

Noeud* NCA(Arbre *arbre, int x, int y)
{
	int estAncetre(Noeud *test, int val)
	{
		if (test->numero == val)
		{
			return TRUE;
		}
		else if (test->filsG == NULL && test->filsD == NULL)
		{
			return FALSE;
		}
		else
		{
			return (estAncetre(test->filsG, val) || estAncetre(test->filsD, val));
		}
	}

	Noeud* NCA(Noeud *noeud, int x, int y)
	{
		if (noeud->filsG == NULL && noeud->filsD == NULL)
		{
			return NULL;
		}
		else if (estAncetre(noeud, x) && estAncetre(noeud, y))
		{
			if ((!(estAncetre(noeud->filsG, x) && estAncetre(noeud->filsG, y))) && (!(estAncetre(noeud->filsD, x) && estAncetre(noeud->filsD, y))))
			{
				return noeud;
			}
			else
			{
				if (NCA(noeud->filsG, x, y) == NULL)
				{
					return NCA(noeud->filsD, x, y);
				}
				else
				{
					return NCA(noeud->filsG, x, y);
				}
			}
		}
		else
		{
			return NULL;
		}
	}

	majArbre(arbre);
	return NCA(arbre->racine, x, y);
}

int adjacentsA(Arbre *arbre, int x, int y)
{
	int taille = 0;
	int **matrice = NULL;
	int reponse = FALSE;
	int i = 0;

	majArbre(arbre);

	taille = arbre->nbFeuilles + 1;
	matrice = calloc(taille, sizeof(int *));
	for (i = 0; i < taille; i++)
	{
		matrice[i] = calloc(taille, sizeof(int));
	}

	arbreEnMatrice(taille, arbre, matrice);
	reponse = adjacentsM(taille, matrice, x, y);
	for (i = 0; i < taille; i++)
	{
		free(matrice[i]);
	}
	free(matrice);
	return reponse;
}

void voisinsA(Arbre *arbre, int x)
{
	int taille = 0;
	int **matrice = NULL;
	int i = 0;

	majArbre(arbre);

	taille = arbre->nbFeuilles + 1;
	matrice = calloc(taille, sizeof(int *));
	for (i = 0; i < taille; i++)
	{
		matrice[i] = calloc(taille, sizeof(int));
	}

	arbreEnMatrice(taille, arbre, matrice);
	voisinsM(taille, matrice, x);
	
	for (i = 0; i < taille; i++)
	{
		free(matrice[i]);
	}
	free(matrice);
}

int clotureTransitiveCompleteA(Arbre *arbre)
{
	int taille = 0;
	int **matrice = NULL;
	int **trans = NULL;
	int reponse = FALSE;
	int i = 0;

	majArbre(arbre);

	taille = arbre->nbFeuilles + 1;
	matrice = calloc(taille, sizeof(int *));
	trans = calloc(taille, sizeof(int*));

	for (i = 0; i < taille; i++)
	{
		matrice[i] = calloc(taille, sizeof(int));
		trans[i] = calloc(taille, sizeof(int));
	}

	fermetureTransitiveM(taille, matrice, trans);
	reponse = estCompleteM(taille, trans);

	for (i = 0; i < taille; i++)
	{
		free(matrice[i]);
		free(trans[i]);
	}
	free(matrice);
	free(trans);

	return reponse;
}
