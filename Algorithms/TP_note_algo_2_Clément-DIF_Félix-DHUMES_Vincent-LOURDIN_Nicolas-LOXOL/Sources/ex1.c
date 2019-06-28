#include "test.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

void remplirM(int taille, int **matrice)
{
	int i = 0;
	int j = 0;
	int val = 0;
	matrice[0][0] = 0;		// Initialisation du coin supérieur gauche même ci ce dernier ne sera pas utilisé
	for (i = 1; i < taille; i++)
	{
		matrice[0][i] = i;	// Etiquetage des colonnes pour l'affichage
		matrice[i][0] = i;	// Etiquetage des lignes pour l'affichage
	}
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			printf("Si l'on a %dR%d, veuillez rentrer 1, sinon, merci de rentrer 1: ", i, j);
			scanf("%d", &val);
			matrice[i][j] = val;
		}
	}
}

void remplirL(int taille, Element* liste[taille])
{
	int i = 0;
	int j = 0;
	int val = 0;
	
	/* On entre les y en relation avec les x */
	Element *elemCourant = NULL;

	for(i = 1; i < taille; i++)
	{
		printf("Entrez la valeur du premier élément en relation avec %d (-1 si aucun) : ", i);
		scanf("%d",&val);

		if(val == -1)
		{
			continue;		// On passe à l'élément suivant
		}

		liste[i] = (Element*) calloc(1, sizeof(Element));
		elemCourant = liste[i];
		elemCourant->val = val;
		elemCourant->suivant = NULL;

		for(j = 0; j < taille - 2; j++)		// On retire 1 à la taille de l'ensemble car le premier élément en relation a déjà été renseigné
		{
			printf("Entrez la valeur de l'élément suivant en relation avec %d : ", i);
			scanf("%d",&val);

			if(val == -1)
			{
				break;
			}

			elemCourant->suivant = (Element*) calloc(1, sizeof(Element));
			elemCourant = elemCourant->suivant;
			elemCourant->val = val;
			elemCourant->suivant = NULL;
		}
	}
}

void afficheM(int taille, int **matrice)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < taille; i++)
	{
		for(j = 0; j < taille; j++)
		{
			if(i == 0 && j == 0)
			{
				printf("\\ ");
			}
			else
			{
				printf("%d ",matrice[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
}

void afficheL(int taille, Element* liste[taille])
{
	int i = 0;
	Element *elemCourant = NULL;
	for(i = 1; i < taille; i++)
	{
		printf("%d : ", i);
		elemCourant = liste[i];
		while(elemCourant != NULL)
		{
			printf("%d - ", elemCourant->val);
			elemCourant = elemCourant->suivant;
		}
		printf("//\n");
	}
}

void copierM(int taille, int **src, int **dest)
{
	int i = 0;
	int j = 0;

	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			dest[i][j] = src[i][j];
		}
	}
}

void copierL(int taille, Element* src[taille], Element* dest[taille])
{
	int i = 0;
	int j = 0;

	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			definirL(taille, dest, i, j, adjacentsL(taille, src, i, j));
		}
	}
}

void definirL(int taille, Element* liste[taille], int x, int y, int existe)
{
	Element *elemCourant = NULL;
	Element *tmp = NULL;
	if (existe)
	{
		if (liste[x] == NULL)
		{
			liste[x] = (Element*) calloc(1, sizeof(Element));
			liste[x]->val = y;
			liste[x]->suivant = NULL;
			return;
		}
		elemCourant = liste[x];
		while (elemCourant->val != y && elemCourant->suivant != NULL)
		{
			elemCourant = elemCourant->suivant;
		}
		if (elemCourant->val == y)
		{
			return;
		}
		else
		{
			elemCourant->suivant = (Element*) calloc(1, sizeof(Element));
			elemCourant = elemCourant->suivant;
			elemCourant->val = y;
			elemCourant->suivant = NULL;
		}
	}
	else
	{
		if (liste[x] != NULL)
		{
			if (liste[x]->val == y)
			{
				free(liste[x]);
				liste[x] = NULL;
				return;
			}
			elemCourant = liste[x];
			while (elemCourant->suivant != NULL)
			{
				if (elemCourant->suivant->val == y)
				{
					tmp = elemCourant->suivant->suivant;
					free(elemCourant->suivant);
					elemCourant->suivant = tmp;
					return;
				}
				elemCourant = elemCourant->suivant;
			}
		}
	}
}

void libererL(int taille, Element* liste[taille])
{
	int i = 0;
	Element *elemCourant = NULL;
	Element *tmp = NULL;
	for(i = 1; i < taille; i++)
	{
		if (liste[i] != NULL)
		{
			elemCourant = liste[i]->suivant;
			free(liste[i]);
			liste[i] = NULL;
		}
		while(elemCourant != NULL)
		{
			tmp = elemCourant;
			elemCourant = tmp->suivant;
			free(tmp);
		}
	}
}

int adjacentsM(int taille, int **matrice, int x, int y)
{
	/* Teste si x et y sont adjacents dans une relation en codage matrice */

	if (x < 1 || x >= taille || y < 1 || y >= taille)
	{
		fprintf(stderr, "Erreur adjacentsM : paramètres invalides\n");
	}
	return (matrice[x][y]);
}

int adjacentsL(int taille, Element* liste[taille], int x, int y)
{
	/* Teste si x et y sont adjacents dans une relation en codage liste chaînée */

	Element *elemCourant = liste[x];
	while(elemCourant != NULL)
	{
		if(elemCourant->val == y)
		{
			return TRUE;
		}
		elemCourant = elemCourant->suivant;
	}
	return FALSE;
}

void voisinsM(int taille, int **matrice, int x)
{
	int j = 0;
	printf("voisins de %d : ", x);
	for(j = 1; j < taille; j++)
	{
		if(matrice[x][j])
		{
			printf("%d - ", matrice[0][j]);
		}
	}
	printf("//\n");
}

void voisinsL(int taille, Element* liste[taille], int x)
{
	Element *elemCourant = liste[x];
	printf("voisins de %d : ", x);
	while(elemCourant != NULL)
	{
		printf("%d - ", elemCourant->val);
		elemCourant = elemCourant->suivant;
	}
	printf("//\n");
}


void fermetureTransitiveM(int taille, int **matrice, int **trans)
{
	int k = 0;
	int i = 0;
	int j = 0;
	copierM(taille, matrice, trans);

	for (k = 1; k < taille; k++)
	{
		for (i = 1; i < taille; i++)
		{
			for (j = 1; j < taille; j++)
			{
				trans[i][j] = trans[i][j] || (trans[i][k] && trans[k][j]);
			}
		}
	}
}

void fermetureTransitiveL(int taille, Element* liste[taille], Element **trans)
{
	int k = 0;
	int i = 0;
	int j = 0;
	copierL(taille, liste, liste);

	for (k = 1; k < taille; k++)
	{
		for (i = 1; i < taille; i++)
		{
			for (j = 1; j < taille; j++)
			{
				definirL(taille, trans, i, j, adjacentsL(taille, trans, i, j) || (adjacentsL(taille, trans, i, k) && adjacentsL(taille, trans, k, j)));
			}
		}
	}
}

void matriceEnListe(int taille, int **matrice, Element* liste[taille])
{
	/* liste est un tableau de pointeurs sur la structure Element */
	int i = 0;
	int j = 0;
	int premierInsere = FALSE;
	
	/* On entre les y en relation avec les x */
	Element *elemCourant = NULL;

	for(i = 1; i < taille; i++)
	{
		for(j = 1; j < taille; j++)		// On retire 1 à la taille de l'ensemble car le premier élément en relation a déjà été renseigné
		{
			if(!premierInsere && matrice[i][j])
			{
				liste[i] = (Element*) calloc(1, sizeof(Element));
				elemCourant = liste[i];
				elemCourant->val = j;
				elemCourant->suivant = NULL;
			}
			else if (premierInsere && matrice[i][j])
			{
				elemCourant->suivant = (Element*) calloc(1, sizeof(Element));
				elemCourant = elemCourant->suivant;
				elemCourant->val = j;
				elemCourant->suivant = NULL;
			}
		}
	}	
}
