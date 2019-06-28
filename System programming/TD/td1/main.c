#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

void test(double a);

int main()
{
	double n = 0.5;
	test(n);

	return EXIT_SUCCESS;
}

void copie_en_majuscule(char *fname, char *fcopie)
{
	char buffer;
	FILE *f1 = fopen(fname, "r");
	FILE *f2 = fopen(fcopie, "w");
	while(fread(&buffer, sizeof(char), 1, f1) != 0)
	{
		if (buffer >= 97 && buffer <= 122)
		{
			buffer = buffer - 32;
		}
		fwrite(&buffer, sizeof(char), 1, f2);
	}
	fclose(f1);
	fclose(f2);
}

void test(int a)
{
	printf("%d\n", a);
	return;
}