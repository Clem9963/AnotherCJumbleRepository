#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define TAILLE_ENSEMBLE 4

/* Déclaration des différentes structures qui seront utilisées tout
au long du programme pour les différents codages. */

struct Element {
	int val;
	struct Element *suivant;
};
typedef struct Element Element;

struct Noeud {
	char etiquette;	// Etiquette vaut '1', '+' ou '*''
	int numero;		// Le numéro, 0 si le noeud n'est pas une feuille 
	struct Noeud *filsG;
	struct Noeud *filsD;
};
typedef struct Noeud Noeud;

struct Arbre {
	Noeud *racine;
	int nbFeuilles;
};
typedef struct Arbre Arbre;

void genererMatrice();
	/* Fonction de test sur les relations en codage matrice. */
void genererListe();
	/* Fonction de test sur les relations en codage liste. */
void genererArbre();
	/* Fonction de test sur les relations en codage arbre. */

#endif