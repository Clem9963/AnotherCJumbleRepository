#ifndef EX1_H_INCLUDED
#define EX1_H_INCLUDED

/*
Exo4.1
q1
matrice : taille(V) * taille(V) = O(taille(V)^2)
liste : pour chaque element deux cases : un pour la valeur et un pour l'adresse du suivant et le cas extreme c'est quand
tous les elements sont en relationa avec tous donc => 2 * taille(V) * taille(V) donc O(taille(V)^2) comme la matrice
q2 CODE
q3 adjacentM O(1), une seule instruction
adjacentL au max, O(taille(V)), si tous les y sont en relation avec le x recherche
voisinM O(taille(V))
voisinL complexite est la taille de la liste renvoyee par voisinsL() donc au pire O(taille(V)) mais on peut aussi dire O(taille(voisinL(x)))
*/

/* Prototypes de manipulation basique des relations (affichage, création, etc.) */

void remplirM(int taille, int **matrice);
	/* Initialisation d'un relation en codage matrice.
	Fait appel à l'utilisateur.
	ATTENTION : pas d'allocation dynamique dans cette fonction ! */
void remplirL(int taille, Element* liste[taille]);
	/* Initialisation d'un relation en codage liste chaînée.
	Fait appel à l'utilisateur.
	L'argument liste est un tableau de pointeurs sur la structure Element
	ATTENTION : pas d'allocation dynamique de la liste dans cette fonction !
	Par contre, l'allocation singulière pour chaque élément est gérée par la fonction. */
void afficheM(int taille, int **matrice);
	/* Affichage formaté d'une relation en codage matrice */
void afficheL(int taille, Element* liste[taille]);
	/* Affichage formaté d'une relation en codage liste chaînée */
void copierM(int taille, int **src, int **dest);
	/* Copie une relation en codage matrice dans une autre en codage matrice.
	ATTENTION : pas d'allocation dynamique dans cette fonction ! */
void copierL(int taille, Element* src[taille], Element* dest[taille]);
	/* Copie une relation en codage liste dans une autre en codage liste.
	ATTENTION : pas d'allocation dynamique dans cette fonction !
	L'allocation (ou la libération) singulière pour l'élément est gérée par la fonction. */
void definirL(int taille, Element* liste[taille], int x, int y, int existe);
	/* Définit si une relation existe entre x et y dans celle codée en liste chaînée 
	L'allocation (ou la libération) singulière pour l'élément est gérée par la fonction. */
void libererL(int taille, Element* liste[taille]);
	/* Permet de purger la liste c'est-à-dire libérer la place prise par chaque élément.
	Tous les pointeurs de la liste sont en conséquence définis à NULL.
	Cette fonction doit obligatoirement être appelée avant la libération manuelle de la liste. */

/* Prototypes de transformation et de sélection */

int adjacentsM(int taille, int **matrice, int x, int y);
	/* Teste si deux éléments sont en relation en codage matrice. */
int adjacentsL(int taille, Element* liste[taille], int x, int y);
	/* Teste si deux éléments sont en relation en codage liste. */
void voisinsM(int taille, int **matrice, int x);
	/* Affiche les voisins de x dans la relation en codage matrice fournie en argument */
void voisinsL(int taille, Element* liste[taille], int x);
	/* Affiche les voisins de x dans la relation en codage liste fournie en argument */
void fermetureTransitiveM(int taille, int **matrice, int **trans);
	/* Génère la fermeture transitive de la relation en codage matrice.
	ATTENTION : pas d'allocation dynamique dans cette fonction ! */
void fermetureTransitiveL(int taille, Element* liste[taille], Element **trans);
	/* Génère la fermeture transitive de la relation en codage matrice.
	ATTENTION : pas d'allocation dynamique dans cette fonction ! */
void matriceEnListe(int taille, int **matrice, Element* liste[taille]);
	/* Copie un codage matrice d'une relation en matrice dans une codage liste
	ATTENTION : pas d'allocation dynamique dans cette fonction, seuls les ajouts et libérations
	singuliers sont gérés ! */

#endif
