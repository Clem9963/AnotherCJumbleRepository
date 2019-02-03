#ifndef EX4_H_INCLUDED
#define EX4_H_INCLUDED

Noeud* NCA(Arbre *arbre, int x, int y);
	/* Retourne NCA(x, y) selon deux numéros de deux feuilles x et y dans l'arbre fourni en argument.*/
int adjacentsA(Arbre *arbre, int x, int y);
	/* Teste si deux éléments sont en relation en codage arbre. */
void voisinsA(Arbre *arbre, int x);
	/* Affiche les voisins de x dans la relation en codage arbre fournie en argument. */
int clotureTransitiveCompleteA(Arbre *arbre);
	/* Teste si la clôture transitive de la relation en codage arbre fournie en argument est complète */

#endif
