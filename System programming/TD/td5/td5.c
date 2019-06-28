#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

void *compter_mots_suspects(char **phrase, char **mots_suspects)
{
	int i = 0;
	int suspect = FALSE;
	int j = 0;
	int *compteur = calloc(1, sizeof(int));
	while(phrase[i] != NULL)
	{
		while(!suspect && mots_suspects[j] != NULL)
		{
			if (strcmp(phrase[i], mots_suspects[i]))
			{
				suspect = TRUE;
				(*compteur)++;
			}
			j++;
		}
		j = 0;
		i++;
		suspect = FALSE;
	}
	return (void *)compteur;
}

int score_texte(char ***texte, char **mots_suspects)
{
	int score_global = 0;
	for (int i = 0;  texte[i] != NULL; i++)
	{
		int *score_phrase = compter_mots_suspects(texte[i], mots_suspects);
		score_global += *score_phrase;
		free(score_phrase);
	}
	return score_global;
}



int main()
{
	return 0;
}