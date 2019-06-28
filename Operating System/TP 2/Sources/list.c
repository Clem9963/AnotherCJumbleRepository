#include "test-gc.h"
#include "list.h"
#include "memory.h"

List* initList()
{
	List *list = malloc(sizeof(List));

	if (list == NULL)
	{
		fprintf(stderr, "initList Error : malloc Error\n");
		exit(EXIT_FAILURE);	
	}

	list->first = NULL;
	return list;
}

void addListHead(List *list, void *nwValue)
{
	Element *new = NULL;

	if (list == NULL)
	{
		fprintf(stderr, "addListHead Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}

	new =  malloc(sizeof(Element));

	if (new == NULL)
	{
		fprintf(stderr, "addListHead Error : malloc Error\n");
		exit(EXIT_FAILURE);	
	}

	new->value = nwValue;
	new->next = list->first;
	list->first = new;
}

void addListAfter(List *list, void *nwValue, Element *element)
{
	Element *new = NULL;
	Element *current = NULL;

	if (list == NULL)
	{
		fprintf(stderr, "addListAfter Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}

	new = malloc(sizeof(Element));

	if (new == NULL)
	{
		fprintf(stderr, "addListAfter Error : malloc Error\n");
		exit(EXIT_FAILURE);	
	}

	current = list->first;
	while (current != element && current != NULL)
	{
		current = current->next;
	}
	if (current == NULL)
	{
		fprintf(stderr, "addListAfter Error : Argument invalide\n");
		return;
	}

	new->next = current->next;
	new->value = nwValue;
	current->next = new;
}

void* freeListElement(List *list, Element *element)
{
	Element *current_element = NULL;
	Element *tmp = NULL;
	void *value = NULL;

	if (list == NULL)
	{
		fprintf(stderr, "freeListElement Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}

	if (element == list->first)
	{
		tmp = list->first;
		value = tmp->value;
		list->first = list->first->next;
		free(tmp);
		return value;
	}

	current_element = list->first;

	while (current_element->next != element && current_element->next != NULL)
	{
		current_element = current_element->next;
	}
	if (current_element->next == NULL)
	{
		fprintf(stderr, "freeListElement Error : Argument invalide\n");
		return NULL;
	}

	tmp = current_element->next;
	value = tmp->value;
	current_element->next = current_element->next->next;
	free(tmp);
	return value;
}

void freeListAll(List *list)
{
	Element *current_element = NULL;
	Element *tmp = NULL;

	if (list == NULL)
	{
		fprintf(stderr, "freeListAll Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}

	current_element = list->first;

	while(current_element != NULL)
	{
		tmp = current_element;
		current_element = current_element->next;
		free(tmp);
	}
	free(list);
}
