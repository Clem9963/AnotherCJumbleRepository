#include <stdio.h>
#include <stdlib.h>

/* DECLARATION DES STRUCTURES */

struct Element
{
	int number;
	struct Element *next;
};
typedef struct Element Element;

struct Stack
{
	struct Element *first;
};
typedef struct Stack Stack;

/* PROTOTYPES */

/* Fonctions de maipulation des piles */
Stack initStack();
void pile(Stack *stack, int nwNb);
int unpack(Stack *stack);

/* Fonctions de traitement */
void reverseArray(int *array, int length);

/* Fonctions d'affichage */
void displayArray(int *array, int length);

/* POINT D'ENTREE */

int main()
{
	int array[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	reverseArray(array, sizeof(array)/sizeof(array[0]));
	displayArray(array, sizeof(array)/sizeof(array[0]));
	return EXIT_SUCCESS;
}

Stack initStack()
{
	Stack stack = {NULL};
	return stack;
}

void pile(Stack *stack, int nwNb)
{
	Element *new = malloc(sizeof(Element*));

	if (stack == NULL)
	{
		fprintf(stderr, "pile Error : Illegal Argument");
		free(new);
		exit(EXIT_FAILURE);
	}
	if (new == NULL)
	{
		fprintf(stderr, "malloc Error");
		exit(EXIT_FAILURE);	
	}

	new->number = nwNb;
	new->next = stack->first;
	stack->first = new;
}

int unpack(Stack *stack)
{
	int unpacked_number = 0;
	Element *unpacked_element = NULL;
	
	if (stack == NULL)
	{
		fprintf(stderr, "unpack Error : Illegal Argument");
		exit(EXIT_FAILURE);
	}

	unpacked_element = stack->first;

	if (stack != NULL && stack->first != NULL)
	{
		unpacked_number = unpacked_element->number;
		stack->first = unpacked_element->next;
		free(unpacked_element);
	}
	return unpacked_number;
}

void reverseArray(int *array, int length)
{
	int i = 0;
	Stack stack = initStack();

	for (i = 0; i < length; i++)
	{
		pile(&stack, array[i]);
	}
	for (i = 0; i < length; i++)
	{
		array[i] = unpack(&stack);
	}
}

void displayArray(int *array, int length)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		printf("%d\n", array[i]);
	}
}
