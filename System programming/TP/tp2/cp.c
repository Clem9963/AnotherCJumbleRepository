#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char ** argv)
{
	FILE *f_source;
	FILE *f_dest;
	char buffer;
	int nb_blocs;

	if (argc != 3)
	{
		printf("Les arguments sont invalides\n");
		exit(1);
	}

	f_source = fopen(argv[1], "r");
	
	if (f_source == NULL)
	{
		printf("cp: %s: Aucun fichier ou dossier de ce type\n", argv[1]);
		exit(1);
	}

	f_dest = fopen(argv[2], "w");

	nb_blocs = fread(&buffer, sizeof(char), 1, f_source);
	while(nb_blocs > 0)
	{
		fputc(buffer, f_dest);
		nb_blocs = fread(&buffer, sizeof(char), 1, f_source);
	}

	return 0;
}