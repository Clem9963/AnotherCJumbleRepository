#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void displaySpecialChar(const char *str);
void spaceFormatting(const char *src, char *dest, int dest_len);
int wordCount(const char *src, char arg);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Error main : IllegalArgument\n");
		exit(EXIT_FAILURE);
	}

	printf("%d\n", wordCount(argv[1], 'w'));

	return EXIT_SUCCESS;
}

void displaySpecialChar(const char *str)
{
	int i = 0;
	int tab_nb = 0;
	int space_nb = 0;
	int eol_nb = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		switch (str[i])
		{
			case '\n':
			{
				eol_nb++;
				break;
			}
			case '\t':
			{
				tab_nb++;
				break;
			}
			case ' ':
			{
				space_nb++;
				break;
			}
		}	
	}

	printf("La chaîne de caractère passée en argument contient\n%d caractères de fin de ligne\n%d tabulations\n%d espaces\n", eol_nb, tab_nb, space_nb);
}

void spaceFormatting(const char *src, char *dest, int dest_len)
{
	int i = 0;
	int j = 0;
	for (i = 0; src[i] != '\0' && j < dest_len; i++)
	{
		if (src[i] == ' ')
		{
			dest[j] = src[i];
			while (src[i+1] == ' ' && src[i] != '\0')
			{
				i++;
			}
		}
		else
		{
			dest[j] = src[i];
		}
		j++;
	}
}

int wordCount(const char *src, char arg)
{
	int i = 0;
	int count = 1;

	if (arg == 'l')
	{
		for (i = 0; src[i] != '\0'; i++)
		{
			if (src[i] == '\n')
			{
				count++;
			}
		}
	}
	else if (arg == 'w')
	{
		char *tmp = calloc(strlen(src)+1, sizeof(char));
		spaceFormatting(src, tmp, strlen(tmp)+1);
		for (i = 0; tmp[i] != '\0'; i++)
		{
			if ((src[i] == ' ' || src[i] == '\n') && src[i+1] != '\0')
			{
				count++;
			}
		}
		free(tmp);
	}
	else if (arg == 'c')
	{
		count = 0;
		for (i = 0; src[i] != '\0'; i++)
		{
			count++;
		}
	}
	else
	{
		fprintf(stderr, "Error wordCount : IllegalArgument\n");
		count = -1;
	}
	return count;
}