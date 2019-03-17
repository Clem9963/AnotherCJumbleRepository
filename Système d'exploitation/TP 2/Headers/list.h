#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "test-gc.h"

/* DECLARATION DES STRUCTURES */

struct Element
{
	void *value;
	struct Element *next;
};
typedef struct Element Element;

struct List
{
	struct Element *first;
};
typedef struct List List;

/* PROTOTYPES */

/* Fonctions de maipulation des listes */

List* initList();
/* Initialise une liste vide allouée dynamqieument */

void addListHead(List *list, void *nwValue);
/* Ajoute un élément alloué dynamiquement en tête de liste qui pointe vers la valeur "nwValue". */

void addListAfter(List *list, void *nwValue, Element *element);
/* Ajoute un élément, alloué dynamiquement et qui pointe vers la valeur "nwValue", dans la liste, après l'élément spécifié en paramètre. */

void* freeListElement(List *list, Element *element);
/* Supprime l'élément "element" dans la liste "list". Elle retourne la valeur pointée par l'élément supprimé. */

void freeListAll(List *list);
/* Supprime tous les élément de la liste puis, supprime la liste elle-même. */

#endif
