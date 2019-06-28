#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "test-gc.h"
#include "list.h"

/* DECLARATION DES STRUCTURES */

struct Block
{
	void *first_word;
	size_t size;
	int used;
	void *reference;
};
typedef struct Block Block;

/* PROTOTYPES */

/* Fonctions de maipulation de la mémoire */

List* initMemory();
/* Initialise une liste allouée dynamiquement qui ne contient qu'un seul bloc de mémoire non utilisé. */

void* allocationMemoire(List *blocks, size_t size);
/* Alloue de la mémoire de taille demandée dans la liste de blocs fournie en paramètre et retourne l’adresse de début du bloc alloué. */

void liberationMemoire(List *blocks, void *address);
/* Supprime le bloc dont l'adresse du premier mot correspond à "address" dans la liste de blocs "blocks" */

void memoryFreeingAll(List *blocks);
/* Supprime toute la mémoire de la liste des blocs puis supprime la liste elle-même. */

void associeBlocs(List *blocks, void *src, void *dest);
/* Créé une association depuis le bloc dont l'adresse du premier mot est "src" vers le bloc dont l'adresse du premier mot est "dest". */

List *initialiseMemoireAleatoirement();
/* Initialise de la mémoire aléatoirement, en appelant 10 fois la fonction allocationMemoire sur des tailles aléatoires
et en appelant associeBlocs et liberationMemoire 1 fois chacune. */

void displayMemory(List *blocks);
/* Permet d'afficher la mémoire pour le debug. */

int applyModifications(List *blocks);
/* Invite l'utilisateur à "agir" sur la mémoire. Renvoie le booléen FALSE si et seulement si l'utilisateur souhaite quitter. */

#endif
