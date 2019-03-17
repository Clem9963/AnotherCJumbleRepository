#include "test-gc.h"
#include "list.h"
#include "memory.h"

List* initMemory()
{
	List *list = initList();
	Block *block = malloc(sizeof(Block));

	if (block == NULL)
	{
		fprintf(stderr, "initMemory Error : malloc Error\n");
		exit(EXIT_FAILURE);	
	}

	block->first_word = malloc(WORD_NUMBER * WORD_SIZE);
	block->size = WORD_NUMBER;
	block->used = FALSE;
	block->reference = NULL;

	if (block->first_word == NULL)
	{
		fprintf(stderr, "initMemory Error : malloc Error\n");
		exit(EXIT_FAILURE);	
	}

	addListHead(list, (void *)block);

	return list;
}

void* allocationMemoire(List *blocks, size_t size)
{
	Element *current_element = NULL;
	Block *new_block = NULL;
	size_t words_nb = ceil(size / (double)WORD_SIZE);

	if (blocks == NULL)
	{
		fprintf(stderr, "allocationMemoire Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}
	else if (blocks->first == NULL)
	{
		fprintf(stderr, "allocationMemoire Error : Pas de mémoire disponible\n");
		exit(EXIT_FAILURE);
	}
	if ((long)size < 1)
	{
		fprintf(stderr, "allocationMemoire Error : La taille demandée est invalide\n");
		return NULL;
	}

	current_element = blocks->first;
	if (((Block*)current_element->value)->size > words_nb && !((Block*)current_element->value)->used)
	{
		new_block = malloc(sizeof(Block));
		if (new_block == NULL)
		{
			fprintf(stderr, "allocationMemoire Error : malloc Error\n");
			exit(EXIT_FAILURE);
		}
		new_block->first_word = ((Block*)current_element->value)->first_word;
		new_block->size = words_nb;
		new_block->used = TRUE;
		new_block->reference = NULL;

		((Block*)current_element->value)->first_word = new_block->first_word + words_nb;
		((Block*)current_element->value)->size = ((Block*)current_element->value)->size - words_nb;

		addListHead(blocks, new_block);

		return new_block->first_word;
	}
	else if (((Block*)current_element->value)->size == words_nb && !((Block*)current_element->value)->used)
	{
		((Block*)current_element->value)->used = TRUE;
		return ((Block*)current_element->value)->first_word;
	}

	while (current_element->next != NULL && (((Block*)current_element->next->value)->size < words_nb || ((Block*)current_element->next->value)->used))
	{
		current_element = current_element->next;
	}

	if (current_element->next == NULL)	// Aucun bloc ne contient assez de mémoire contiguë pour "l'allocation" de mémoire
	{
		fprintf(stderr, "allocationMemoire Error : Trop de mémoire déjà utilisée\n");
		exit(EXIT_FAILURE);
	}

	if (((Block*)current_element->next->value)->size > words_nb)
	{
		new_block = malloc(sizeof(Block));
		if (new_block == NULL)
		{
			fprintf(stderr, "allocationMemoire Error : malloc Error\n");
			exit(EXIT_FAILURE);
		}
		new_block->first_word = ((Block*)current_element->next->value)->first_word;
		new_block->size = words_nb;
		new_block->used = TRUE;
		new_block->reference = NULL;

		((Block*)current_element->next->value)->first_word = new_block->first_word + words_nb;
		((Block*)current_element->next->value)->size = ((Block*)current_element->next->value)->size - words_nb;

		addListAfter(blocks, new_block, current_element);

		return new_block->first_word;
	}
	else if (((Block*)current_element->next->value)->size == words_nb)
	{
		((Block*)current_element->next->value)->used = TRUE;
		return ((Block*)current_element->next->value)->first_word;
	}

	return NULL;
}

void liberationMemoire(List *blocks, void *address)
{
	Element *current_element = NULL;
	
	if (blocks == NULL)
	{
		fprintf(stderr, "liberationMemoire Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}
	else if (blocks->first == NULL)
	{
		fprintf(stderr, "liberationMemoire Error : Pas de mémoire disponible\n");
		exit(EXIT_FAILURE);
	}

	current_element = blocks->first;
	if (((Block*)current_element->value)->first_word == address)
	{
		if (!((Block*)current_element->value)->used)
		{
			fprintf(stderr, "liberationMemoire Error : Ce bloc a déjà été libéré ou n'a jamais été alloué auparavant\n");
			return;
		}
		((Block*)current_element->value)->used = FALSE;
		if (current_element->next != NULL && !((Block*)current_element->next->value)->used)
		{
			((Block*)current_element->value)->size += ((Block*)current_element->next->value)->size;
			free(current_element->next->value);
			freeListElement(blocks, current_element->next);
		}
		return;
	}

	while (current_element->next != NULL && ((Block*)current_element->next->value)->first_word != address)
	{
		current_element = current_element->next;
	}

	if (current_element->next == NULL)
	{
		fprintf(stderr, "liberationMemoire Error : Aucun bloc ne correspond à l'adresse\n");
		return;
	}

	if (!((Block*)current_element->next->value)->used)
	{
		fprintf(stderr, "liberationMemoire Error : Ce bloc a déjà été libéré ou n'a jamais été alloué auparavant\n");
		return;
	}
	((Block*)current_element->next->value)->used = FALSE;
	if (current_element->next->next != NULL && !((Block*)current_element->next->next->value)->used)
	{
		((Block*)current_element->next->value)->size += ((Block*)current_element->next->next->value)->size;
		free(current_element->next->next->value);
		freeListElement(blocks, current_element->next->next);
	}
	if (!((Block*)current_element->value)->used)
	{
		((Block*)current_element->next->value)->size += ((Block*)current_element->value)->size;
		((Block*)current_element->next->value)->first_word = ((Block*)current_element->value)->first_word;
		free(current_element->value);
		freeListElement(blocks, current_element);
	}
	return;
}

void memoryFreeingAll(List *blocks)
{
	Element *current_element = NULL;

	free(((Block*)blocks->first->value)->first_word);	// Libération de toute la mémoire avant libération des blocs
	current_element = blocks->first;

	while(current_element != NULL)
	{
		free(current_element->value);					// Libération de chaque bloc
		current_element = current_element->next;
	}

	freeListAll(blocks);								// Libération des éléments qui pointaient sur les blocs
}

void associeBlocs(List *blocks, void *src, void *dest)
{
	Element *current_element = NULL;
	Element *current_element2 = NULL;
	
	if (blocks == NULL)
	{
		fprintf(stderr, "associeBlocs Error : Argument invalide\n");
		exit(EXIT_FAILURE);
	}
	else if (blocks->first == NULL)
	{
		fprintf(stderr, "associeBlocs Error : Pas de mémoire disponible\n");
		exit(EXIT_FAILURE);
	}

	current_element = blocks->first;
	current_element2 = blocks->first;

	while (current_element != NULL)
	{
		if (((Block*)current_element->value)->first_word == src)
		{
			if (((Block*)current_element->value)->size != 1)
			{
				fprintf(stderr, "associeBlocs Error : Taille invalide\n");
				return;
			}
			if (!((Block*)current_element->value)->used)
			{
				fprintf(stderr, "associeBlocs Error : Le bloc source n'est pas alloué\n");
				return;
			}
			while (current_element2 != NULL)
			{
				if (((Block*)current_element2->value)->first_word == dest)
				{
					((Block*)current_element->value)->reference = ((Block*)current_element2->value)->first_word;
					return;
				}
				current_element2 = current_element2->next; 
			}
			fprintf(stderr, "associeBlocs Error : Une adresse est invalide\n");
			return;
		}
		current_element = current_element->next; 
	}
	fprintf(stderr, "associeBlocs Error : Une adresse est invalide\n");
	return;
}

List *initialiseMemoireAleatoirement()
{
	int i = 0;
	List *blocks = initMemory();
	void *addresses[10];

	srand(time(NULL));		// Initialisation de rand pour la génération de nombres aléatoires

	addresses[i] = allocationMemoire(blocks, 1);	//	Le bloc source de taille 1 pour un exemple de référence
	for (i = 1; i < 10; i++)
	{
		addresses[i] = allocationMemoire(blocks, rand()%8 + 1);
	}

	associeBlocs(blocks, addresses[0], addresses[rand()%9 + 1]);
	liberationMemoire(blocks, addresses[1]);		// Exemple de libération de mémoire

	return blocks;
}

void displayMemory(List *blocks)
{
	Element *current_element = NULL;
	int number = 0;

	printf("\n\n~~~ DEBUG UTILISATION MÉMOIRE - NOMBRE DE MOTS : %d - TAILLE D'UN MOT : %d Octet(s) ~~~\n\n", WORD_NUMBER, WORD_SIZE);
	printf("N° du bloc\tAdresse de début\tAdresse de fin\t\tUtilisé\t\tRéférence\tTaille du bloc\n\n");

	current_element = blocks->first;
	while (current_element != NULL)
	{
		printf("     %d\t\t", number);
		printf("%p   --->\t", ((Block*)current_element->value)->first_word);
		printf("%p\t\t", ((Block*)current_element->value)->first_word + ((Block*)current_element->value)->size - 1);
		printf("%s\t\t", (((Block*)current_element->value)->used)?"Yes":"No");
		if (((Block*)current_element->value)->reference != NULL)
		{
			printf("%p\t", ((Block*)current_element->value)->reference);
		}
		else
		{
			printf("NULL\t\t");
		}
		printf("%ld\n", ((Block*)current_element->value)->size);

		current_element = current_element->next;
		number++;
	}

	printf("\nN° du bloc\tAdresse de début\tAdresse de fin\t\tUtilisé\t\tRéférence\tTaille du bloc\n\n");
}

int applyModifications(List *blocks)
{
	int intAnswer = 0;
	void *ptrAnswer1 = NULL;
	void *ptrAnswer2 = NULL;

	printf("Que souhaitez vous faire ?\n");
	printf("1 : Allouer de la mémoire\n");
	printf("2 : Libérer de la mémoire\n");
	printf("3 : Effectuer une référence\n");
	printf("0 : Quitter\n\n> ");
	scanf("%d", &intAnswer);

	if (intAnswer == 0)
	{
		return FALSE;
	}
	if (intAnswer == 1)
	{
		printf("\nVeuillez entrer la taille en octets\n> ");
		scanf("%d", &intAnswer);
		allocationMemoire(blocks, intAnswer);
		return TRUE;
	}
	else if (intAnswer == 2)
	{
		printf("\nVeuillez entrer l'adresse du premier mot (adresse de départ) du bloc à libérer\n> ");
		scanf("%p", &ptrAnswer1);
		liberationMemoire(blocks, ptrAnswer1);
		return TRUE;
	}
	else if (intAnswer == 3)
	{
		printf("\nVeuillez entrer l'adresse du premier mot (adresse de départ) du bloc source obligatoirement de taille 1\n> ");
		scanf("%p", &ptrAnswer1);
		printf("\nVeuillez entrer l'adresse du premier mot (adresse de départ) du bloc destination\n> ");
		scanf("%p", &ptrAnswer2);
		associeBlocs(blocks, ptrAnswer1, ptrAnswer2);
		return TRUE;
	}
	else
	{
		fprintf(stderr, "La réponse n'est pas valide.\n\n");
		return TRUE;
	}
}
