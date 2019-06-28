#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
 
void cat_sans_parametres();
void cat_fichier(char *fname);

int main(int argc, char ** argv)
{
	int i = 0;

	/*
	printf("Affichage de argc : ");
	printf("%d\n\n", argc);
	
	printf("Affichage des élément de argv\n");
	for (i = 0 ; i < argc ; i++)
	{
		printf("%s\n", argv[i]);
	}
	*/
	
	for (i = 1 ; i < argc ; i++)
	{
		cat_fichier(argv[i]);
	}
	return 0;
}

void cat_sans_parametres()
{
	while(TRUE)
	{
		char buffer[100];
		fgets(buffer, sizeof(buffer), stdin);
		printf("%s", buffer);
	}
}

void cat_fichier(char *fname)
{
	char buffer;
	int nb_blocs = 0;
	FILE *f;

	f = fopen(fname, "r");
	if (f == NULL)
	{
		printf("cat: %s: Aucun fichier ou dossier de ce type\n", fname);
		exit(1);
	}
	else
	{
		nb_blocs = fread(&buffer, sizeof(char), 1, f);
		while(nb_blocs > 0)
		{
			printf("%c", buffer);
			nb_blocs = fread(&buffer, sizeof(char), 1, f);
		}
	printf("\n");
	}
}