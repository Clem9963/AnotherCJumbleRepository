#ifndef EX2_H_INCLUDED
#define EX2_H_INCLUDED

/* Prototypes de catégorisation des relation */

int estIndependanteM(int taille, int **matrice);
	/* Teste si la relation en codage matrice est indépendante */
int estIndependanteL(int taille, Element* liste[taille]);
	/* Teste si la relation en codage liste est indépendante */
int estCompleteM(int taille, int **matrice);
	/* Teste si la relation en codage matrice est complète */
int estCompleteL(int taille, Element* liste[taille]);
	/* Teste si la relation en codage liste est complète */
int estArborescenteM(int taille, int **matrice);
	/* Teste si la relation en codage matrice est arborescente */
int estArborescenteL(int taille, Element* liste[taille]);
	/* Teste si la relation en codage liste est arborescente */
int estP4M(int taille, int **matrice, const int tabElements[4]);
	/* Teste si la relation en codage matrice est P4
	pour les elements fournis en paramètres dans tabElements */
int estP4L(int taille, Element* liste[taille], const int tabElements[4]);
	/* Teste si la relation en codage liste est P4
	pour les elements fournis en paramètres dans tabElements */
int estCoGrapheM(int taille, int **matrice);
	/* Teste si la relation en codage matrice est un co-graphe */
int estCoGrapheL(int taille, Element* liste[taille]);
	/* Teste si la relation en codage liste est un co-graphe */

#endif