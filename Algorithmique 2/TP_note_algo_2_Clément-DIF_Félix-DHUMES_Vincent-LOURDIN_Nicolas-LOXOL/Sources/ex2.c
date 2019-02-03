#include "test.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

int estIndependanteM(int taille, int **matrice)
{
	int i = 0;
	int j = 0;
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (matrice[i][j]) // Si l'on a iRj
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

int estIndependanteL(int taille, Element* liste[taille])
{
	int i = 0;
	for (i = 1; i < taille; i++)
	{
		if (liste[i] != NULL) // Si on trouve une seule liste non vide, cela veut dire qu'il y a une relation et donc R n'est pas indépendante
		{
			return FALSE;
		}
	}
	return TRUE;
}

int estCompleteM(int taille, int **matrice)
{
	int i = 0;
	int j = 0;
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if ((i != j && !adjacentsM(taille, matrice, i, j)) || (i == j && adjacentsM(taille, matrice, i, j)))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

int estCompleteL(int taille, Element* liste[taille])
{
	int i = 0;
	int j = 0;
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if ((i != j && !adjacentsL(taille, liste, i, j)) || (i == j && adjacentsL(taille, liste, i, j)))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

int estArborescenteM(int taille, int **matrice)
{
	int **trans = NULL;
	int compteur = 0;
	int i = 0;
	int j = 0;

	trans = calloc(taille, sizeof(int*));
	for (i = 0; i < taille; i++)
	{
		trans[i] = calloc(taille, sizeof(int));
	}

	copierM(taille, matrice, trans);

	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (adjacentsM(taille, trans, i, j))
			{
				compteur++;
			}
		}
	}

	if (compteur == taille - 2)
	{
		fermetureTransitiveM(taille, matrice, trans);
		return estCompleteM(taille, trans);
	}

	for (i = 0; i < taille; i++)
	{
		free(trans[i]);
	}
	free(trans);

	return FALSE;
}

int estArborescenteL(int taille, Element* liste[taille])
{
	Element** trans = NULL;
	int compteur = 0;
	int i = 0;
	int j = 0;

	trans = calloc(taille, sizeof(Element*));

	copierL(taille, liste, trans);

	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (adjacentsL(taille, trans, i, j))
			{
				compteur++;
			}
		}
	}

	if (compteur == taille - 2)
	{
		fermetureTransitiveL(taille, liste, trans);
		return estCompleteL(taille, trans);
	}

	libererL(taille, trans);
	free(trans);

	return FALSE;
}

int estP4M(int taille, int **matrice, const int tabElements[4])
{
	int elemDeb = 0;
	int elemFin = 0;
	int elemPrec = 0;
	int compteurElements = 0;
	int compteurEnRelation = 0;
	int iter1 = 0;
	int iter2 = 0;
	int index = 0;

	if (taille - 1 < 4)
	{
		return FALSE;
	}

	for (iter1 = 0; iter1 < 4; iter1++)
	{
		for (iter2 = 0; iter2 < 4; iter2++)
		{
			if (adjacentsM(taille, matrice, tabElements[iter1], tabElements[iter2]))
			{
				compteurEnRelation++;
			}
		}
		if (compteurEnRelation == 1)
		{
			if (elemDeb == 0)
			{
				elemDeb = tabElements[iter1];
				compteurElements++;
			}
			else if (elemFin == 0)
			{
				elemFin = tabElements[iter1];
				compteurElements++;
			}
			else
			{
				return FALSE;	// Il ne peut y avoir que deux éléments en relation avec un seul élément
			}
		}
		if (compteurEnRelation == 2)
		{
			compteurElements++;
		}
		else if (compteurEnRelation >= 3)		// L'élément en question est en relation avec trop d'autres éléments
		{
			return FALSE;			// On est donc en l'absence d'une relation P4
		}
		compteurEnRelation = 0;
	}

	if (compteurElements < 4)			// Si on a moins de 4 éléments qui sont en relation
	{
		return FALSE;
	}
	else
	{
		index = elemDeb;
		for (iter1 = 0; iter1 < 3; iter1++)		// On n'aura pas besoin de vérifier le dernier indice car celui-ci sera vérifié avec l'avant-dernier
		{
			for (iter2 = 0; iter2 < 4; iter2++)
			{
				if (tabElements[iter2] == elemPrec)
				{
					continue;
				}
				if (adjacentsM(taille, matrice, index, tabElements[iter2]))
				{
					if (!adjacentsM(taille, matrice, tabElements[iter2], index))
					{
						return FALSE;
					}
					else
					{
						index = tabElements[iter2];
					}
				}
			}
		}
	}
	return TRUE;
}

int estP4L(int taille, Element* liste[taille], const int tabElements[4])
{
	int elemDeb = 0;
	int elemFin = 0;
	int elemPrec = 0;
	int compteurElements = 0;
	int compteurEnRelation = 0;
	int iter1 = 0;
	int iter2 = 0;
	int index = 0;

	if (taille - 1 < 4)
	{
		return FALSE;
	}

	for (iter1 = 0; iter1 < 4; iter1++)
	{
		for (iter2 = 0; iter2 < 4; iter2++)
		{
			if (adjacentsL(taille, liste, tabElements[iter1], tabElements[iter2]))
			{
				compteurEnRelation++;
			}
		}
		if (compteurEnRelation == 1)
		{
			if (elemDeb == 0)
			{
				elemDeb = tabElements[iter1];
				compteurElements++;
			}
			else if (elemFin == 0)
			{
				elemFin = tabElements[iter1];
				compteurElements++;
			}
			else
			{
				return FALSE;	// Il ne peut y avoir que deux éléments en relation avec un seul élément
			}
		}
		if (compteurEnRelation == 2)
		{
			compteurElements++;
		}
		else if (compteurEnRelation >= 3)		// L'élément en question est en relation avec trop d'autres éléments
		{
			return FALSE;			// On est donc en l'absence d'une relation P4
		}
		compteurEnRelation = 0;
	}

	if (compteurElements < 4)			// Si on a moins de 4 éléments qui sont en relation
	{
		return FALSE;
	}
	else
	{
		index = elemDeb;
		for (iter1 = 0; iter1 < 3; iter1++)		// On n'aura pas besoin de vérifier le dernier indice car celui-ci sera vérifié avec l'avant-dernier
		{
			for (iter2 = 0; iter2 < 4; iter2++)
			{
				if (tabElements[iter2] == elemPrec)
				{
					continue;
				}
				if (adjacentsL(taille, liste, index, tabElements[iter2]))
				{
					if (!adjacentsL(taille, liste, tabElements[iter2], index))
					{
						return FALSE;
					}
					else
					{
						index = tabElements[iter2];
					}
				}
			}
		}
	}
	return TRUE;
}

int estCoGrapheM(int taille, int **matrice)
{
	int tabElements[4] = {1,2,3,4};
	int finEnumeration = FALSE;
	int valide = FALSE;
	int result = TRUE;

	if (taille - 1 < 4)
	{
		return FALSE;
	}

	while (!finEnumeration && result)
	{
		result = result && !estP4M(taille, matrice, tabElements);
		valide = FALSE;
		while (!valide)
		{
			if (tabElements[3] == taille - 1)
			{
				tabElements[3] = 1;
				if (tabElements[2] == taille - 1)
				{
					tabElements[2] = 1;
					if (tabElements[1] == taille - 1)
					{
						tabElements[1] = 1;
						if (tabElements[0] == taille - 1)
						{
							finEnumeration = TRUE;
						}
						else
						{
							tabElements[0]++;
						}
					}
					else
					{
						tabElements[1]++;
					}
				}
				else
				{
					tabElements[2]++;
				}
			}
			else
			{
				tabElements[3]++;
			}
			valide = 	(tabElements[0] != tabElements[1] && tabElements[0] != tabElements[2] && tabElements[0] != tabElements[3] &&
						tabElements[1] != tabElements[2] && tabElements[1] != tabElements[3] &&
						tabElements[2] != tabElements[3]);
		}
	}
	return result;
}

int estCoGrapheL(int taille, Element* liste[taille])
{
	int tabElements[4] = {1,2,3,4};
	int finEnumeration = FALSE;
	int valide = FALSE;
	int result = TRUE;

	if (taille - 1 < 4)
	{
		return FALSE;
	}

	while (!finEnumeration && result)
	{
		result = result && !estP4L(taille, liste, tabElements);
		valide = FALSE;
		while (!valide)
		{
			if (tabElements[3] == taille - 1)
			{
				tabElements[3] = 1;
				if (tabElements[2] == taille - 1)
				{
					tabElements[2] = 1;
					if (tabElements[1] == taille - 1)
					{
						tabElements[1] = 1;
						if (tabElements[0] == taille - 1)
						{
							finEnumeration = TRUE;
						}
						else
						{
							tabElements[0]++;
						}
					}
					else
					{
						tabElements[1]++;
					}
				}
				else
				{
					tabElements[2]++;
				}
			}
			else
			{
				tabElements[3]++;
			}
			valide = 	(tabElements[0] != tabElements[1] && tabElements[0] != tabElements[2] && tabElements[0] != tabElements[3] &&
						tabElements[1] != tabElements[2] && tabElements[1] != tabElements[3] &&
						tabElements[2] != tabElements[3]);
		}
	}
	return result;
}
