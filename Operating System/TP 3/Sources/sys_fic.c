#include "sys_fic.h"

/* Voir le header pour la description des fonctions */

FileSystem* initFileSystem()
{
	FileSystem *fs = NULL;
	fs = malloc(sizeof(FileSystem));

	if (fs == NULL)
	{
		fprintf(stderr, "Erreur dans initDesciptor : Echec d'allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}

	fs->blocks_number = 0;
	fs->inodes_number = 0;
	fs->blocks_bitmap = NULL;
	fs->inodes_bitmap = NULL;
	fs->blocks_array = NULL;
	fs->inodes_array = NULL;

	return fs;
}

void freeFileSystem(FileSystem *fs)
{
	if (fs != NULL)
	{
		if (fs->blocks_bitmap != NULL)
		{
			free(fs->blocks_bitmap);
		}
		if (fs->inodes_bitmap != NULL)
		{
			free(fs->inodes_bitmap);
		}
		if (fs->blocks_array != NULL)
		{
			free(fs->blocks_array);
		}
		if (fs->inodes_array != NULL)
		{
			free(fs->inodes_array);
		}
		free(fs);
	}
	else
	{
		fprintf(stderr, "Erreur dans freeFileSystem : Le système de fichiers fourni en argument est invalide ou a déjà été libéré.\n");
	}
}

int loadFileSystem(FileSystem *fs, char *path)
{
	FILE *file;

	file = fopen(path, "r");

	if (file == NULL)
	{
		fprintf(stderr, "Erreur dans loadFileSystem : le chemin path entré en argument est invalide.\n");
		return FALSE;
	}
	else
	{
		/* Placement au début du fichier */
		fseek(file, 0, SEEK_SET);

		/* Lecture des informations primaires */
		fread(&fs->blocks_number, sizeof(fs->blocks_number), 1, file);
		fread(&fs->inodes_number, sizeof(fs->inodes_number), 1, file);

		/* Allocations dynamiques de mémoire */
		fs->blocks_bitmap = calloc(fs->blocks_number/8, sizeof(uint8_t));
		fs->inodes_bitmap = calloc(fs->inodes_number/8, sizeof(uint8_t));
		fs->blocks_array = malloc(fs->blocks_number * sizeof(Block));
		fs->inodes_array = malloc(fs->inodes_number * sizeof(Inode));

		/* Lecture des données */
		fread(fs->blocks_bitmap, sizeof(uint8_t), fs->blocks_number/8, file);
		fread(fs->inodes_bitmap, sizeof(uint8_t), fs->inodes_number/8, file);
		fread(fs->inodes_array, sizeof(Inode), fs->inodes_number, file);
		fread(fs->blocks_array, sizeof(Block), fs->blocks_number, file);

		fclose(file);
		return TRUE;
	}
}

void saveFileSystem(FileSystem *fs, char *path)
{
	FILE *file;

	file = fopen(path, "w");

	if (file == NULL)
	{
		fprintf(stderr, "Erreur dans saveFileSystem : le chemin path entré en argument est invalide.\n");
	}
	else
	{
		/* Placement au début du fichier */
		fseek(file, 0, SEEK_SET);

		/* Ecriture des informations primaires */
		fwrite(&fs->blocks_number, sizeof(fs->blocks_number), 1, file);
		fwrite(&fs->inodes_number, sizeof(fs->inodes_number), 1, file);

		/* Ecriture des données */
		fwrite(fs->blocks_bitmap, sizeof(uint8_t), fs->blocks_number/8, file);
		fwrite(fs->inodes_bitmap, sizeof(uint8_t), fs->inodes_number/8, file);
		fwrite(fs->inodes_array, sizeof(Inode), fs->inodes_number, file);
		fwrite(fs->blocks_array, sizeof(Block), fs->blocks_number, file);

		fclose(file);
	}

}

int allocateBlock(FileSystem *fs)
{
	int i = 0;
	for (i = 0; i < fs->blocks_number; i++)
	{
		if(!getUsedFlagBlock(fs, i))			// Un bloc non utilisé a été trouvé
		{
			setUsedFlagBlock(fs, i, TRUE);		// On le marque comme étant utilisé
			return i; 
		}
	}
	return -1;
}

int allocateInode(FileSystem *fs)
{
	int i = 0;
	for (i = 0; i < fs->inodes_number; i++)
	{
		if(!getUsedFlagInode(fs, i))			// Un inode non utilisé a été trouvé
		{
			setUsedFlagInode(fs, i, TRUE);		// On le marque comme étant utilisé
			return i; 
		}
	}
	return -1;
}

int getUsedInodesNumber(FileSystem *fs)
{
	int i = 0;
	int counter = 0;

	for (i = 0; i < fs->inodes_number; i++)
	{
		if (getBitInByte(fs->inodes_bitmap[i/8], 7 - i%8))	// On regarde la valeur pour cet inode dans le bitmap des inodes utilisés
		{
			counter++;
		}
	}
	return counter;
}

int getUsedFlagInode(FileSystem *fs, int inode_index)
{
	if (inode_index < 0 || inode_index >= fs->inodes_number)
	{
		fprintf(stderr, "Erreur dans getUsedFlagInode : L'indice de l'inode fourni en paramètre est invalide.\n");
		return TRUE;	// On indique que "l'inode" qui n'existe pas effectivement est utilisé par sécurité
	}
	return getBitInByte(fs->inodes_bitmap[inode_index/8], 7 - inode_index%8);
}

void setUsedFlagInode(FileSystem *fs, int inode_index, uint8_t value)
{
	if (inode_index < 0 || inode_index >= fs->inodes_number)
	{
		fprintf(stderr, "Erreur dans setUsedFlagInode : L'indice de l'inode fourni en paramètre est invalide.\n");
	}
	setBitInByte(&fs->inodes_bitmap[inode_index/8], 7 - inode_index%8, value);
}

int getUsedFlagBlock(FileSystem *fs, int block_index)
{
	if (block_index < 0 || block_index >= fs->blocks_number)
	{
		fprintf(stderr, "Erreur dans getUsedFlagBlock : L'indice du bloc fourni en paramètre est invalide.\n");
		return TRUE;	// On indique que "le bloc" qui n'existe pas effectivement est utilisé par sécurité
	}
	return getBitInByte(fs->blocks_bitmap[block_index/8], 7 - block_index%8);
}

void setUsedFlagBlock(FileSystem *fs, int block_index, uint8_t value)
{
	if (block_index < 0 || block_index >= fs->blocks_number)
	{
		fprintf(stderr, "Erreur dans setUsedFlagBlock : L'indice du bloc fourni en paramètre est invalide.\n");
	}
	setBitInByte(&fs->blocks_bitmap[block_index/8], 7 - block_index%8, value);
}

uint8_t getBitInByte(uint8_t byte, uint8_t bit_number)
{
	if (bit_number > 7)
	{
		fprintf(stderr, "Erreur dans getBitInByte : le numéro du bit entré en argument n'est pas valide.\n");
		return 0;
	}
	else
	{
		return (byte >> bit_number) % 2;
	}
}

void setBitInByte(uint8_t *byte, uint8_t bit_number, uint8_t value)
{
	if (bit_number > 7)
	{
		fprintf(stderr, "Erreur dans setBitInByte : le numéro du bit entré en argument n'est pas valide.\n");
	}
	if (value)
	{
		if (!getBitInByte(*byte, bit_number))
		{
			*byte += pow(2, bit_number);
		}
	}
	else
	{
		if (getBitInByte(*byte, bit_number))
		{
			*byte -= pow(2, bit_number);
		}
	}
}

char* getFileName(FileSystem *fs, int inode_index)
{
	if (getUsedFlagInode(fs, inode_index))
	{
		return fs->inodes_array[inode_index].name;
	}
	else
	{
		fprintf(stderr, "Erreur dans getFileName : L'indice de l'inode fourni en paramètre pointe vers un inode non alloué.\n");
		return NULL;
	}
}

uint8_t getFileNameSize(FileSystem *fs, int inode_index)
{
	if (getUsedFlagInode(fs, inode_index))
	{
		return fs->inodes_array[inode_index].name_size;
	}
	else
	{
		fprintf(stderr, "Erreur dans getFileNameSize : L'indice de l'inode fourni en paramètre pointe vers un inode non alloué.\n");
		return 0;
	}
}

uint8_t getFileSize(FileSystem *fs, int inode_index)
{
	if (getUsedFlagInode(fs, inode_index))
	{
		return fs->inodes_array[inode_index].size;
	}
	else
	{
		fprintf(stderr, "Erreur dans getFileSize : L'indice de l'inode fourni en paramètre pointe vers un inode non alloué.\n");
		return 0;
	}
}

uint8_t* getBlocksIndex(FileSystem *fs, int inode_index)
{
	if (getUsedFlagInode(fs, inode_index))
	{
		return fs->inodes_array[inode_index].blocks_index;
	}
	else
	{
		fprintf(stderr, "Erreur dans getBlocksIndex : L'indice de l'inode fourni en paramètre pointe vers un inode non alloué.\n");
		return NULL;
	}
}

uint8_t* getBlockAddress(FileSystem *fs, int block_index)
{
	if (getUsedFlagBlock(fs, block_index))
	{
		return fs->blocks_array[block_index].content;
	}
	else
	{
		fprintf(stderr, "Erreur dans getBlockAddress : L'indice de l'inode fourni en paramètre pointe vers un inode non alloué.\n");
		return NULL;
	}
}

void displayBlockContent(uint8_t *content)
{
	int i = 0;
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		printf("%c", content[i]);
	}
}
