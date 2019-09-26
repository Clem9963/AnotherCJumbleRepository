#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DECLARATION DES STRUCTURES */

struct Item
{
	int number;
	struct Item *next;
};
typedef struct Item Item;

struct List
{
	struct Item *first;
};
typedef struct List List;

/* FONCTIONS DE MANIPULATION DES LISTES */
List initList();
void add(List *list, int nwNb);
void removeItem(List *list);

/* POINT D'ENTREE */

int main()
{
	/* code */
	return EXIT_SUCCESS;
}

List initList()
{
	List list = {NULL};
	Item *item = malloc(sizeof(Item*));

	if (item == NULL)
	{
		fprintf(stderr, "malloc Error");
		exit(EXIT_FAILURE);
	}

	item->number = 0;
	item->next = NULL;
	list.first = item;

	return list;
}

void add(List *list, int nwNb)
{
	Item *new = malloc(sizeof(Item*));

	if (list == NULL)
	{
		fprintf(stderr, "add Error : Illegal Argument");
		free(new);
		exit(EXIT_FAILURE);
	}

	if (new == NULL)
	{
		fprintf(stderr, "malloc Error");
		exit(EXIT_FAILURE);
	}

	new->number = nwNb;
	new->next = list->first;
	list->first = new;
}

void removeItem(List *list)
{
	if (list == NULL)
	{
		fprintf(stderr, "remove Error : Illegal Argument");
		exit(EXIT_FAILURE);
	}

	if (list->first != NULL)
	{
		Item *to_be_removed = list->first;
		list->first = list->first->next;
		free(to_be_removed);
	}
}
