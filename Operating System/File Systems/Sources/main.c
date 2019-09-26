#include "file_sys.h"
#include "descriptor.h"
#include "sys_call.h"

void flushStdin();
void getFromStdin(char *buffer, int len);
int min(int a, int b);

int main()
{
	FileSystem *fs = NULL;
	Descriptor *current_file = NULL;
	uint8_t *blocks_index = NULL;
	uint8_t *block_content = NULL;
	uint8_t buffer[BLOCK_SIZE + 1];			// Le +1 est obligatoire pour stocker le caractère de fin de chaîne et, en conséquence, simplifer les traitements

	uint8_t quit = FALSE;
	uint8_t valid_answer = FALSE;
	
	int int_answer = 0;
	int whence = 0;
	char str_answer[FILE_NAME_SIZE + 1];	// Le +1 est obligatoire pour stocker le caractère de fin de chaîne et, en conséquence, simplifer les traitements
	char file_system_name[32];
	char full_file_system_name[48];
	
	int i = 0;
	int j = 0;

	fs = initFileSystem();

	while (!valid_answer)
	{
		printf("Veuillez entrer un nom de système de fichier à charger\n");
		printf("Exemple : myFileSystem.fs\n");
		printf("> ");
		getFromStdin(file_system_name, sizeof(file_system_name));
		sprintf(full_file_system_name, "./file-fs/%s", file_system_name);
		
		if (!loadFileSystem(fs, full_file_system_name))
		{
			fprintf(stderr, "Le système de fichier \"%s\" n'a pas pu être chargé\n", file_system_name);
			fprintf(stderr, "Vérifiez s'il-vous-plaît qu'il se trouve dans le dossier file-fs.\n\n");
		}
		else
		{
			fprintf(stderr, "Chargement du système de fichier \"%s\" réussi avec succès\n\n", file_system_name);
			valid_answer = TRUE;
		}
	}

	printf("~~~ INFORMATIONS GLOBALES DU SYSTEME DE FICHIERS ~~~\n");
	printf("Nombre de blocs disponibles : %d\n", fs->blocks_number);
	printf("Nombre d'inodes disponibles : %d\n", fs->inodes_number);
	printf("Nombre d'inodes utilisés : %d\n", getUsedInodesNumber(fs));

	while(!quit)
	{
		int_answer = 0;
		valid_answer = FALSE;

		while (!valid_answer)
		{
			printf("\n~~ Etat actuel du système de fichiers ~~\n");
			if (current_file == NULL)
			{
				printf("Aucun fichier n'a été ouvert pour le moment\n\n");
			}
			else
			{
				printf("Le fichier ");
				for (i = 0; i < current_file->inode->name_size; i++)
				{
					printf("%c", current_file->inode->name[i]);
				}
				printf(" est actuellement ouvert.\n");
				printf("Numéro du bloc courant : %d\n", current_file->current_block_number);
				if (current_file->offset == BLOCK_SIZE)
				{
					printf("Offset dans le bloc courant  : EOF (End Of File)\n");
				}
				else
				{
					printf("Offset dans le bloc courant  : %ld\n", current_file->offset);
				}
				printf("Taille totale du fichier : %d\n\n", current_file->inode->size);
			}

			printf("\nVoici les options disponibles (entrez le chiffre correspondant) : \n");
			printf("0  : Quitter le programme (les modifications non sauvegardées seront perdues).\n");
			printf("1  : Créer un nouveau fichier dans le système de fichier.\n");
			printf("2  : Ouvrir un fichier / fermer le fichier actuel.\n");
			printf("3  : Agir sur le fichier ouvert.\n");
			printf("4  : Sauvegarder les modifications.\n");
			printf("42 : Mode Debug\n");
			printf("> ");

			scanf("%d", &int_answer);
			flushStdin();

			if (int_answer < 0 || (int_answer > 4 && int_answer!= 42))
			{
				fprintf(stderr, "L'entier saisi ne correspond pas à une option disponible\n");
			}
			else if (int_answer == 0)
			{
				freeFileSystem(fs);
				if (current_file != NULL)
				{
					closeFile(current_file);
				}
				valid_answer = TRUE;
				return EXIT_SUCCESS;
			}
			else if (int_answer == 1)
			{
				printf("Veuillez entrer le nom de fichier (8 caractères maximum)\n");
				printf("> ");
				getFromStdin(str_answer, FILE_NAME_SIZE + 1);
				printf("Nom du nouveau fichier : %s\n", str_answer);
				printf("Indice de l'inode correspondant : %d\n", createFile(fs, str_answer));
				valid_answer = TRUE;
			}
			else if (int_answer == 2)
			{
				if (current_file == NULL)	// Pas de fichier couramment ouvert
				{
					printf("Veuillez entrer le nom de fichier (8 caractères maximum)\n");
					printf("> ");
					getFromStdin(str_answer, FILE_NAME_SIZE + 1);
					current_file = openFile(fs, str_answer);
					if (current_file != NULL)
					{
						printf("Le fichier %s a été ouvert avec succès !\n", str_answer);
					}
				}
				else						// Un fichier était déjà ouvert. Il faut donc le fermer
				{
					printf("Le fichier ");
					for (i = 0; i < current_file->inode->name_size; i++)
					{
						printf("%c", current_file->inode->name[i]);
					}
					printf(" a été fermé avec succès !\n");
					closeFile(current_file);
					current_file = NULL;
				}
				valid_answer = TRUE;
			}
			else if (int_answer == 3)
			{
				if (current_file == NULL)
				{
					fprintf(stderr, "Aucun fichier n'est actuellement ouvert !\n");
				}
				else
				{
					while (!valid_answer)
					{
						printf("\nVoici les options disponibles (entrez le chiffre correspondant) : \n");
						printf("0 : Annuler.\n");
						printf("1 : Changer le décalage dans le fichier.\n");
						printf("2 : Lire dans le fichier\n");
						printf("3 : Ecrire dans le fichier\n");
						printf("> ");

						scanf("%d", &int_answer);
						flushStdin();

						if (int_answer < 0 || int_answer > 3)
						{
							fprintf(stderr, "L'entier saisi ne correspond pas à une option disponible\n");
						}
						else if (int_answer == 0)
						{
							printf("Retour au menu principal\n");
							valid_answer = TRUE;
						}
						else if (int_answer == 1)
						{
							printf("\nVeuillez entrer la nouvelle position de la tête\n");
							printf("0 : La tête est placée au début du fichier\n");
							printf("1 : La tête conserve la position courante dans le fichier\n");
							printf("2 : La tête est placée à la fin du fichier\n");
							printf("> ");

							scanf("%d", &int_answer);
							flushStdin();

							if (int_answer < 0 || int_answer > 2)
							{
								fprintf(stderr, "Le nombre saisi est incorrect\n");
							}
							else
							{
								whence = int_answer;
								printf("Veuillez entrer le décalage par rapport à la nouvelle position de la tête (positif ou négatif)\n");
								printf("> ");

								scanf("%d", &int_answer);
								flushStdin();

								seekInFile(fs, current_file, int_answer, whence);
								valid_answer = TRUE;
							}
						}
						else if (int_answer == 2)
						{
							printf("Veuillez entrer le nombre d'octets à lire depuis le décalage courant (16 maximum)\n");
							printf("> ");

							scanf("%d", &int_answer);
							flushStdin();

							if (int_answer < 0)
							{
								fprintf(stderr, "Le nombre d'octets est négatif !\n");
							}
							else
							{
								int_answer = min(int_answer, BLOCK_SIZE);

								int_answer = readFile(fs, current_file, buffer, int_answer);	// Le retour de la fonction indique combien de caractères ont été lus
								printf("%d octets lus : ", int_answer);
								for (i = 0; i < int_answer; i++)
								{
									printf("%c", buffer[i]);
								}
								printf("\n");
								valid_answer = TRUE;
							}
						}
						else
						{
							printf("Veuillez entrer la chaîne de caractères à écrire depuis le décalage courant (16 maximum)\n");
							printf("Les caractères n'appartenant pas à la table ASCII ne sont pas conseillés.\n");
							printf("> ");

							getFromStdin((char *)buffer, BLOCK_SIZE + 1);

							printf("%ld octets écrits\n", writeFile(fs, current_file, buffer, (uint8_t)strlen((char *)buffer)));
							valid_answer = TRUE;
						}
					}
				}
			}
			else if (int_answer == 4)
			{
				saveFileSystem(fs, full_file_system_name);
				printf("Modifications sauvegardées avec succès !\n");
				printf("Le fichier myFileSystem a été écrasé !\n");
				valid_answer = TRUE;
			}
			else if (int_answer == 42)
			{
				printf("\n");
				printf("\n");
				printf("~~ La grande question sur la vie, l'univers et le reste ~~\n");
				printf("~~            Ici, vous avez réponse à tout             ~~\n\n");
				for (i = 0; i < fs->inodes_number; i++)
				{
					if (getUsedFlagInode(fs, i))
					{
						printf("Nom du fichier : \t\t");
						for (j = 0; j < getFileNameSize(fs, i); j++)
						{
							printf("%c", getFileName(fs, i)[j]);
						}
						printf("\n");
						printf("Taille du nom du fichier : \t%d\n", getFileNameSize(fs, i));
						printf("Indice de l'inode utilisé : \t%d\n", i);
						printf("Taille du fichier : \t\t%d octets\n", getFileSize(fs, i));

						blocks_index = getBlocksIndex(fs, i);
						printf("Indices des blocs de données : \t");
						for (j = 0; j < BLOCKS_NUMBER; j++)
						{
							if (blocks_index[j] != 0)	// Si le bloc est valide
							{
								printf("%d ", blocks_index[j] - 1);
							}
						}
						printf("\n");

						printf("Blocs de données :\n");
						for (j = 0; j < BLOCKS_NUMBER; j++)
						{
							if (blocks_index[j] != 0)	// Si le bloc est valide
							{
								printf("\tBloc numéro %d : ", j);
								block_content = getBlockAddress(fs, blocks_index[j] - 1);
								displayBlockContent(block_content);
								printf("\n");
							}
						}
						printf("\n\n");
					}
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

void flushStdin()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}
 
void getFromStdin(char *buffer, int len)
{
    char *new_line = NULL;
 
    if (fgets(buffer, len, stdin) != NULL)
    {
        new_line = strchr(buffer, '\n');
        if (new_line != NULL)
        {
            *new_line = '\0';
        }
        else
        {
            flushStdin();
        }
    }
    else
    {
        flushStdin();
    }
}

int min(int a, int b)
{
	return (a < b)?a:b;
}
