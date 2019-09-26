#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char byte;

/* DECLARATION DES STRUCTURES */

struct Item
{
	int coefficient;
	byte degree;
	struct Item *next;
};
typedef struct Item Item;

struct Polynomial
{
	struct Item *first;
};
typedef struct Polynomial Polynomial;

/* FONCTIONS DE MANIPULATION DES POLYNÔMES */
Polynomial initPolynomial();
void removeFirstItem(Polynomial *list);


/* FONCTIONS D'INTERFACE UTIISATEUR */
void addPolynomial(Polynomial *p, char *string)
{
	int i = 0;
	for (i = 0; i < strlen(string) + 1; i++)
	{
		// TODO
	}

}

/* POINT D'ENTREE */

int main()
{
	return EXIT_SUCCESS;
}

Polynomial initPolynomial()
{
	Polynomial p = {NULL};
	return p;
}

void add(Polynomial *p, int coefficient, byte degree)
{
	Item *current = p->first;
	Item *tmp = NULL;

	if (p == NULL)
	{
		fprintf(stderr, "add Error : Illegal Argument.");
		exit(EXIT_FAILURE);
	}
	if (p->first == NULL)
	{
		p->first = malloc(sizeof(Item*));
		p->first->coefficient = coefficient;
		p->first->degree = degree;
	}
	else
	{
		while (current->next != NULL && current->next->degree < degree)
		{
			current = current->next;
		}
		
		if (current->next == NULL)
		{
			current->next = malloc(sizeof(Item*));
			current = current->next;
			current->coefficient = coefficient;
			current->degree = degree;
		}
		else
		{
			tmp = malloc(sizeof(Item*));
			tmp->next = current->next;
			tmp->coefficient = coefficient;
			tmp->degree = degree;
			current->next = tmp;
		}
	}
}

void remove(Polynomial *p, byte degree)
{
	Item *current = p->first;
	Item *tmp = NULL;

	if (p == NULL)
	{
		fprintf(stderr, "remove Error : Illegal Argument.");
		exit(EXIT_FAILURE);
	}

	if (p->first == NULL)
	{
		fprintf(stderr, "remove Error : The polynomial is already empty.");
		exit(EXIT_FAILURE);
	}

	while (current->next != NULL && current->next->degree != degree && current->next->degree < degree)
	{
		current = current->next;
	}
	
	if (current->next == NULL)
	{
		fprintf(stderr, "remove Error : Couldn't find the term.");
		exit(EXIT_FAILURE);
	}
	if (current->next->degree == degree)
	{
		tmp = current->next;
		current->next = current->next->next;;
		free(tmp);
	}
}
