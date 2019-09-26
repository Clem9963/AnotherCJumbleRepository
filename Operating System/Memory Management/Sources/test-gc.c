#include "test-gc.h"
#include "list.h"
#include "memory.h"

int main()
{
	List *memory = NULL;
	
	memory = initialiseMemoireAleatoirement();
	displayMemory(memory);

	while (applyModifications(memory))
	{	
		displayMemory(memory);
	}

	memoryFreeingAll(memory);

	return EXIT_SUCCESS;
}
