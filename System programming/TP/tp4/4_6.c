#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	char* style = NULL;

	style = getenv("STYLE_DE_MUSIQUE");
	if (strncmp(style, "RUMBA", 6) == 0)
	{
		printf("Tout le monde danse la RUMBA !! (Normalement c'est Zumba mais faut pas le dire) ^^\n");
	}
	else
	{
		printf("Stop le Justin Bieber sale débilos\n");
	}
	return EXIT_SUCCESS;
}
