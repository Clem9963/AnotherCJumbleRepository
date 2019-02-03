#ifndef EX3_H_INCLUDED
#define EX3_H_INCLUDED

/* Fonctions de manipulation des arbres */

Arbre* initArbre();
	/* Initialise un arbre à un noeud (relation {1})*/
Arbre* unionArbres(Arbre *arbre1, Arbre *arbre2, char operation);
	/* Créé un nouvel arbre issu de l'union deux deux autres suivant une
	opération '+' ou '*'. Un pointeur sur cet arbre est renvoyé par la fonction.
	D'ailleurs, arbre1 pointe également sur le nouvel arbre tandis que arbre2
	ne pointe plus sur aucun arbre alloué. */
void majArbre(Arbre *arbre);
	/* Permet de mettre à jour l'étiquettage des feuilles de l"arbre. */
void arbreEnMatrice(int taille, Arbre *arbre, int **matrice);
	/* Créé une relation en codage matrice à partir de l'arbre.
	ATTENTION : pas d'allocation dynamique dans cette fonction ! */
void arbreEnListe(int taille, Arbre *arbre, Element *liste[taille]);
	/* Créé une relation en codage liste à partir de l'arbre.
	ATTENTION : pas d'allocation dynamique dans cette fonction !
	L'allocation (ou la libération) singulière pour l'élément est gérée par la fonction. */
void libererArbre(Arbre *arbre);
	/* Permet de libérer l'ensemble de la mémoire consommée par l'arbre. */

#endif
