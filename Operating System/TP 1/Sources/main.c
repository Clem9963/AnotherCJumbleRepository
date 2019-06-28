#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0

typedef char bool;

void getCommand(char *buffer, size_t buffer_size);
int countArgs(char *buffer, int command_number);
int countCommands(char *buffer);
void bufferCopy(char *src, char *dest);

int main()
{
	bool finished = FALSE;
	char buffer[BUFFER_SIZE] = {0};
	char buffer_copy[BUFFER_SIZE] = {0};
	char *cmd = NULL;

	int argsNb = 0;
	int cmdNb = 0;
	char *token;
	char **args = NULL;
	
	pid_t pid = 0;
	int descriptors[2] = {0};

	int i = 0;
	int j = 0;

	pipe(descriptors);

	while (!finished)
	{
		printf("> ");
		getCommand(buffer, BUFFER_SIZE);
		bufferCopy(buffer, buffer_copy);

		pid = waitpid(-1, NULL, WNOHANG);
		while (pid != -1 && pid != 0)
		{
			pid = waitpid(-1, NULL, WNOHANG);
		}

		if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0)
		{
			finished = TRUE;
		}
		else
		{
			cmdNb = countCommands(buffer);
			if (cmdNb == 0)
			{
				continue;
			}
			else if (cmdNb == 1)
			{
				argsNb = countArgs(buffer, 0) + 2;
				args = malloc(argsNb * sizeof(char*));

				token = strtok(buffer, " ");
				for (i = 0; i < argsNb - 1; i++)
				{
					args[i] = strdup(token);
					token = strtok(NULL, " ");
				}
				args[i] = NULL;

				if (strcmp(args[0], "cd") == 0)
				{
					if (args[1] != NULL)
					{
						chdir(args[1]);
					}
				}
				else if (strcmp(args[argsNb - 2], "&") == 0)
				{
					free(args[argsNb - 2]);
					args[argsNb - 2] = NULL;
					pid = fork();
					if (pid == -1)
					{
						fprintf(stderr, "main error : Process creation failure");
					}
					else if (pid == 0)
					{
						execvp(args[0], args);
					}
				}
				else
				{
					pid = fork();
					if (pid == -1)
					{
						fprintf(stderr, "main error : Process creation failure");
					}
					else if (pid == 0)
					{
						execvp(args[0], args);
					}
					else
					{
						waitpid(pid, NULL, 0);
					}
					free(args);
				}
			}
			else
			{
				for (i = 0; i < cmdNb; i++)
				{
					bufferCopy(buffer_copy, buffer);
					argsNb = countArgs(buffer, i) + 2;
					args = malloc(argsNb * sizeof(char*));

					cmd = buffer;
					for (j = 0; j < i; j++)
					{
						cmd = strchr(cmd, '|') + 2;	// On incrémente de deux pour passer l'espace qui suit le pipe
					}
					token = strtok(cmd, " ");
					for (j = 0; j < argsNb - 1; j++)
					{
						args[j] = strdup(token);
						token = strtok(NULL, " ");
					}
					args[j] = NULL;

					if (strcmp(args[0], "cd") == 0)
					{
						if (args[1] != NULL)
						{
							chdir(args[1]);
						}
					}
					else if (strcmp(args[argsNb - 2], "&") == 0)
					{
						if (i != cmdNb - 1)		// S'il ne s'agit de la dernière commande
						{
							fprintf(stderr, "main error : Erreur de syntaxe près de '&'");
						}
						free(args[argsNb - 2]);
						args[argsNb - 2] = NULL;
					}

					pid = fork();
					if (pid == -1)
					{
						fprintf(stderr, "main error : Process creation failure");
					}
					else if (pid == 0)
					{
						if (i == 0)
						{
							close(descriptors[0]);
							dup2(descriptors[1], STDOUT_FILENO);
							execvp(args[0], args);
							close(descriptors[1]);
						}
						else if (i == cmdNb - 1)
						{
							close(descriptors[1]);
							dup2(descriptors[0], STDIN_FILENO);
							execvp(args[0], args);
							close(descriptors[0]);
						}
						else
						{
							// dup2(descriptors[0], STDIN_FILENO);
						}
					}
					else
					{
						waitpid(pid, NULL, 0);
					}
					free(args);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

void getCommand(char *buffer, size_t buffer_size)
{
	int reset = 0;
	char *char_ptr = NULL;

	if (fgets(buffer, buffer_size, stdin) == NULL)
	{
		perror("fgets error");
		exit(errno);
	}

	/* Suppression du saut de ligne final */
	char_ptr = strchr(buffer, '\n');
	if (char_ptr != NULL)
	{
		*char_ptr = '\0';
	}

	/* On flush le flux stdin */
	else
	{
		while (reset != '\n' && reset != EOF)
		{
			reset = getchar();
		}
	}
}

int countArgs(char *buffer, int command_number)
{
	int i = 0;
	int counter = 0;

	while (TRUE)
	{
		if (buffer[i] == '\0')
		{
			return counter;
		}
		else if (buffer[i] == ' ')
		{
			if (buffer[i+1] != '\0')
			{
				if (buffer[i+1] == '|' && buffer[i+2] == ' ')
				{
					if (command_number == 0)
					{
						return counter;
					}
					command_number--;
					i += 2;		// On dépasse le pipe et l'espace pour s'attaquer à la commande suivante
				}
				else if (command_number == 0)
				{
					counter++;
				}
				i++;
			}
		}
		else
		{
			i++;
		}
	}
}

int countCommands(char *buffer)
{
	int i = 0;
	int counter = 0;

	if (buffer[0] != '\0')
	{
		counter++;
	}

	while (TRUE)
	{
		if (buffer[i] == '\0')
		{
			return counter;
		}
		else if (buffer[i] == '|')
		{
			counter++;
			i++;
		}
		else
		{
			i++;
		}
	}
}

void bufferCopy(char *src, char *dest)
{
	int i = 0;

	for (i = 0; src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
}