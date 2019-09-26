#include "test.h"
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"

int main()
{
	genererMatrice(TAILLE_ENSEMBLE + 1);
	genererListe(TAILLE_ENSEMBLE + 1);
	genererArbre();

	return EXIT_SUCCESS;
}

void genererMatrice(int taille)
{
	int tabElements[4] = {1, 2, 3, 4};
	int **matrice = NULL;
	int i = 0;
	int j = 0;

	printf("\n\n~~ Test des fonctions de manipulation des relations en codage matrice ~~\n");
	printf("~~ TAILLE DE L'ENSEMBLE : %d ~~\n", taille - 1);

	matrice = calloc(taille, sizeof(int*));
	for (i = 0; i < taille; i++)
	{
		matrice[i] = calloc(taille, sizeof(int));
	}

	printf("\nMatrice - Remplissage par l'utilisateur\n");
	remplirM(taille, matrice);

	printf("\nMatrice - Affichage\n");
	afficheM(taille, matrice);

	printf("Matrice - Test d'adjacence pour chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (adjacentsM(taille, matrice,i,j))
			{
				printf("%d est adjacent à %d\n", j, i);
			}
		}
	}

	printf("\nMatrice - Voisins de chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		voisinsM(taille, matrice, i);
	}
	printf("\n");

	if (estIndependanteM(taille, matrice))
	{
		printf("La relation est indépendante\n");
	}
	else
	{
		printf("La relation n'est pas indépendante\n");
	}

	if (estCompleteM(taille, matrice))
	{
		printf("La relation est complète\n");
	}
	else
	{
		printf("La relation n'est pas complète\n");
	}

	if (estArborescenteM(taille, matrice))
	{
		printf("La relation est arborescente\n");
	}
	else
	{
		printf("La relation n'est pas arborescente\n");
	}

	if (estP4M(taille, matrice, tabElements))
	{
		printf("La relation est P4\n");
	}
	else
	{
		printf("La relation n'est pas P4\n");
	}

	if (estCoGrapheM(taille, matrice))
	{
		printf("La relation est co-graphe\n");
	}
	else
	{
		printf("La relation n'est co-graphe\n");
	}

	for (i = 0; i < taille; i++)
	{
		free(matrice[i]);
	}
	free(matrice);
}

void genererListe(int taille)
{
	int tabElements[4] = {1, 2, 3, 4};
	Element** liste = NULL;
	int i = 0;
	int j = 0;

	printf("\n\n~~ Test des fonctions de manipulation des relations en codage liste ~~\n");
	printf("~~ TAILLE DE L'ENSEMBLE : %d ~~\n", taille - 1);

	liste = calloc(taille, sizeof(Element*));

	printf("\nListe - Remplissage par l'utilisateur\n");
	remplirL(taille, liste);

	printf("\nListe - Affichage\n");
	afficheL(taille, liste);

	printf("\nListe - Test d'adjacence pour chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (adjacentsL(taille, liste,i,j))
			{
				printf("%d est adjacent à %d\n", j, i);
			}
		}
	}

	printf("\nListe - Voisins de chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		voisinsL(taille, liste, i);
	}
	printf("\n");

	if (estIndependanteL(taille, liste))
	{
		printf("La relation est indépendante\n");
	}
	else
	{
		printf("La relation n'est pas indépendante\n");
	}

	if (estCompleteL(taille, liste))
	{
		printf("La relation est complète\n");
	}
	else
	{
		printf("La relation n'est pas complète\n");
	}

	if (estArborescenteL(taille, liste))
	{
		printf("La relation est arborescente\n");
	}
	else
	{
		printf("La relation n'est pas arborescente\n");
	}

	if (estP4L(taille, liste, tabElements))
	{
		printf("La relation est P4\n");
	}
	else
	{
		printf("La relation n'est pas P4\n");
	}
	
	if (estCoGrapheL(taille, liste))
	{
		printf("La relation est co-graphe\n");
	}
	else
	{
		printf("La relation n'est co-graphe\n");
	}

	libererL(taille, liste);	// On purge la liste
	free(liste);				// Avant de la libérer elle même
}

void genererArbre()
{
	Arbre *arbre = NULL;
	int **matrice = NULL;
	Element** liste = NULL;
	int taille = 0;
	int nb1;
	int nb2;
	int i = 0;
	int j = 0;

	printf("\n\n~~ Test des fonctions de manipulation des relations en codage arbre ~~\n");

	printf("Arbre - Génération arbitraire\n");
	printf("      *\n    /   \\\n   *     *\n  / \\   / \\\n 1   1 1   1\n ^   ^ ^   ^\n 1   2 3   4\n");
	printf("Les numéros des feuilles sont précisés ci-dessus\n");
	arbre = unionArbres(unionArbres(initArbre(), initArbre(), '*'), unionArbres(initArbre(), initArbre(), '*'), '*');
	majArbre(arbre);
	taille = arbre->nbFeuilles + 1;
	printf("\n~~ TAILLE DE L'ENSEMBLE : %d ~~\n", taille - 1);

	matrice = calloc(taille, sizeof(int *));
	liste = calloc(taille, sizeof(Element*));
	for (i = 0; i < taille; i++)
	{
		matrice[i] = calloc(taille, sizeof(int));
	}

	printf("Arbre - Conversion en matrice et affichage\n");
	arbreEnMatrice(taille, arbre, matrice);
	afficheM(taille, matrice);

	printf("Arbre - Conversion en liste et affichage\n");
	arbreEnListe(taille, arbre, liste);
	afficheL(taille, liste);

	printf("\nArbre - NCA\n");
	printf("Veuillez entrer les numéros des feuilles dont vous voulez connaître les ancêtres entre\n");
	printf("1 et %d. Vous pouvez les entrer de cette manière :nb1 nb2\n", arbre->nbFeuilles);
	scanf("%d %d", &nb1, &nb2);
	printf("Voici l'étiquette du NCA : %c\n", NCA(arbre, nb1, nb2)->etiquette);

	printf("\nArbre - Test d'adjacence pour chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		for (j = 1; j < taille; j++)
		{
			if (adjacentsA(arbre, i, j))
			{
				printf("%d est adjacent à %d\n", j, i);
			}
		}
	}

	printf("\nArbre - Voisins de chaque élément\n");
	for (i = 1; i < taille; i++)
	{
		voisinsA(arbre, i);
	}
	printf("\n");

	if (clotureTransitiveCompleteA(arbre))
	{
		printf("La clôture transitive de la relation est complète\n");
	}
	else
	{
		printf("La clôture transitive de la relation n'est pas complète\n");
	}

	for (i = 0; i < taille; i++)
	{
		free(matrice[i]);
	}
	free(matrice);

	libererL(taille, liste);	// On purge la liste
	free(liste);				// Avant de la libérer elle même

	libererArbre(arbre);
}
