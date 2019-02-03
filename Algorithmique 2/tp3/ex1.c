#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/* DECLARATION DES STRUCTURES */
struct Node
{
	int value;
	struct Node *first_son;
	struct Node *brother;
};
typedef struct Node Node;

struct Tree
{
	struct Node *root;
};
typedef struct Tree Tree;

struct Element
{
	struct Node *node;
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
void pile(Stack *stack, Node *nwNode);
Node* unpack(Stack *stack);

/* Fonctions de maipulation des arbres */
Tree generateTree();
void displayTree(Tree *tree);

int main()
{
	Tree tree = generateTree();
	displayTree(&tree);
	return EXIT_SUCCESS;
}

Stack initStack()
{
	Stack stack = {NULL};
	return stack;
}

void pile(Stack *stack, Node *nwNode)
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

	new->node = nwNode;
	new->next = stack->first;
	stack->first = new;
}

Node* unpack(Stack *stack)
{
	Node *unpacked_node = NULL;
	Element *unpacked_element = NULL;
	
	if (stack == NULL)
	{
		fprintf(stderr, "unpack Error : Illegal Argument");
		exit(EXIT_FAILURE);
	}

	unpacked_element = stack->first;

	if (stack != NULL && stack->first != NULL)
	{
		unpacked_node = unpacked_element->node;
		stack->first = unpacked_element->next;
		free(unpacked_element);
	}
	return unpacked_node;
}

Tree generateTree()
{
	Tree tree = {NULL};
	tree.root = calloc(1, sizeof(Node));
	tree.root->value = 1;
	tree.root->first_son = calloc(1, sizeof(Node));
	tree.root->brother = NULL;

	tree.root->first_son->value = 2;
	tree.root->first_son->first_son = calloc(1, sizeof(Node));
	tree.root->first_son->brother = calloc(1, sizeof(Node));

	tree.root->first_son->first_son->value = 5;
	tree.root->first_son->first_son->first_son = NULL;
	tree.root->first_son->first_son->brother = calloc(1, sizeof(Node));

	tree.root->first_son->first_son->brother->value = 6;
	tree.root->first_son->first_son->brother->first_son = NULL;
	tree.root->first_son->first_son->brother->brother = NULL;	


	tree.root->first_son->brother->value = 3;
	tree.root->first_son->brother->brother = calloc(1,sizeof(Node));
	tree.root->first_son->brother->first_son = NULL;

	tree.root->first_son->brother->brother->value = 4;
	tree.root->first_son->brother->brother->brother = NULL;
	tree.root->first_son->brother->brother->first_son = NULL;

	return tree;
}

void addNodeToABR(Node *nwNode, Tree *tree)
{
	int finished = FALSE;
	Node *current = tree->root;

	while (!finished)
	{
		if (nwNode->value <= current->value)
		{
			if (current->first_son == NULL)
			{
				current->first_son = nwNode;
			}
			else if (current->first_son->value <= current->value)
			{
				current = current->first_son;
			}
			else if
			{
				nwNode->brother = current->first_son;
				current->first_son = nwNode;
			}
		}
		else
		{
			if (current->first_son == NULL)
			{
				current->first_son = nwNode;
			}
			else if (current->first_son->value > current->value)
			{
				current = current->first_son;
			}
			else if
			{
				nwNode->brother = current->first_son;
				current->first_son = nwNode;
			}

		}

		if (current->brother != NULL)
		{
			pile(&stack, current->brother);
		}

		if (current->first_son != NULL)
		{
			current = current->first_son;
		}
		else
		{
			current = unpack(&stack);
			if (current == NULL)
			{
				finished = TRUE;
			}
		}
	}
}

void displayTree(Tree *tree)
{
	Stack stack = initStack();
	int finished = FALSE;
	Node *current = tree->root;

	while (!finished)
	{
		printf("%d\n", current->value);

		if (current->brother != NULL)
		{
			pile(&stack, current->brother);
		}

		if (current->first_son != NULL)
		{
			current = current->first_son;
		}
		else
		{
			current = unpack(&stack);
			if (current == NULL)
			{
				finished = TRUE;
			}
		}
	}
}