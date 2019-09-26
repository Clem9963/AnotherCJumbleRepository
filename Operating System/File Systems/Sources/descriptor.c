#include "file_sys.h"
#include "descriptor.h"

Descriptor* initDesciptor(FileSystem *fs, int inode_index)
{
	Descriptor* fd = NULL;

	if (getUsedFlagInode(fs, inode_index))	// On vérifie que l'inode soit bien utilisé
	{
		fd = malloc(sizeof(Descriptor));
		if (fd == NULL)
		{
			fprintf(stderr, "Erreur dans initDesciptor : échec d'allocation dynamique.\n");
		}
		else
		{
			fd->inode = &fs->inodes_array[inode_index];
			fd->current_block_number = 0;
			fd->offset = 0;
			updateCache(fs, fd);
		}
	}
	else
	{
		fprintf(stderr, "Erreur dans initDesciptor : L'indice de l'inode fourni est invalide.\n");
	}

	return fd;
}

void freeDescriptor(Descriptor *fd)
{
	if (fd != NULL)
	{
		free(fd);
	}
	else
	{
		fprintf(stderr, "Erreur dans freeDescriptor : Le descriptor fourni en paramètre n'est pas valide.\nIl a peut-être déjà été libéré.\n");
	}
}

void updateCache(FileSystem *fs, Descriptor* fd)
{
	if (fd->inode->blocks_index[fd->current_block_number] == 0)	// Si le numéro du bloc courant correspond à un bloc non alloué
	{
		fprintf(stderr, "Erreur dans updateCache : Impossible de mettre à jour le cache.\nLe numéro du bloc courant ne correspond pas à un bloc valide.");
	}
	else
	{
		/* On copie le contenu du bloc en cache (mise à  jour) */
		memcpy(fd->cache.content, getBlockAddress(fs, fd->inode->blocks_index[fd->current_block_number] - 1), BLOCK_SIZE);
	}
}

void dseek(FileSystem *fs, Descriptor* fd, off_t offset, int whence)
{
	if (whence == SEEK_CUR)
	{
		offset = fd->offset + BLOCK_SIZE * fd->current_block_number + offset;
	}
	else if (whence == SEEK_END)
	{
		/* Même si la taille n'est pas indicée à zéro, on affecte cette valeur pour offset pour éviter d'écraser le dernier
		caractère lors de la prochaine écriture */
		offset = (off_t)(fd->inode->size + offset);
	}
	else if (whence != SEEK_SET)
	{
		fprintf(stderr, "Erreur dans dseek : L'argument whence est invalide.\n");
	}

	if (offset < 0 || offset > fd->inode->size)
	{
		fprintf(stderr, "Erreur dans dseek : La nouvelle position de la tête est invalide.\n");
	}
	else if (offset == fd->inode->size && fd->inode->size%BLOCK_SIZE == 0)
	{
		/* L'offset est placé juste après le dernier caractère du fichier.
		Il prend la valeur spéciale End Of File (entier 16 en dur) */
		fd->offset = BLOCK_SIZE;
		fd->current_block_number = offset/BLOCK_SIZE - 1;
		updateCache(fs, fd);
	}
	else
	{
		fd->offset = offset%BLOCK_SIZE;
		fd->current_block_number = offset/BLOCK_SIZE;
		updateCache(fs, fd);
	}
}

size_t dread(FileSystem *fs, Descriptor *fd, uint8_t *dest, size_t length)
{
	size_t bytes_read = 0;
	while (bytes_read < length)
	{
		if (fd->offset == BLOCK_SIZE)	// On arrive à la fin du bloc courant
		{
			if (fd->current_block_number + 1 >= BLOCKS_NUMBER || fd->inode->blocks_index[fd->current_block_number + 1] == 0)	// Impossible de continuer la lecture
			{
				/* On quitte la lecture en laissant "fd->offset" garder la valeur spéciale EOF (End Of File) */
				return bytes_read;
			}
			else	// Nécessite un basculement sur le bloc suivant
			{
				fd->current_block_number++;
				fd->offset = 0;
				updateCache(fs, fd);

				/* Poursuite de la lecture */
				dest[bytes_read] = fd->cache.content[fd->offset];
				fd->offset++;
				bytes_read++;
			}
		}
		else if (fd->current_block_number * BLOCK_SIZE + fd->offset + 1 > fd->inode->size)		// Le caractère que l'on souhaitrait lire n'est pas effectivement dans le contenu du fichier
		{
			return bytes_read;
		}
		else	// Conditions normales, on poursuit la lecture
		{
			dest[bytes_read] = fd->cache.content[fd->offset];
			fd->offset++;
			bytes_read++;
			if (fd->offset == BLOCK_SIZE && fd->current_block_number + 1 < BLOCKS_NUMBER && fd->inode->blocks_index[fd->current_block_number + 1] != 0)
			{
				/* On peut directement basculer sur le bloc suivant qui est alloué */
				fd->current_block_number++;
				fd->offset = 0;
				updateCache(fs, fd);
			}
		}
	}
	return bytes_read;
}

size_t dwrite(FileSystem *fs, Descriptor *fd, uint8_t *src, size_t length)
{
	size_t bytes_written = 0;
	while (bytes_written < length)
	{
		if (fd->offset < BLOCK_SIZE)
		{
			if (fd->current_block_number * BLOCK_SIZE + fd->offset + 1 > fd->inode->size)	// On ajoute 1 car la taille ne commence pas à zéro
			{
				/* On va augmenter la taille du fichier car on va ajouter un caractère à la fin */
				fd->inode->size = fd->current_block_number * BLOCK_SIZE + fd->offset + 1;	// Modification de la taille du fichier en conséquence
			}
			getBlockAddress(fs, fd->inode->blocks_index[fd->current_block_number] - 1)[fd->offset] = src[bytes_written];	// Ecriture du caractère
			bytes_written++;
			fd->offset++;
			if (fd->offset == BLOCK_SIZE && fd->current_block_number + 1 < BLOCKS_NUMBER && fd->inode->blocks_index[fd->current_block_number + 1] != 0)
			{
				/* On peut directement basculer sur le bloc suivant qui est alloué */
				fd->current_block_number++;
				fd->offset = 0;
				updateCache(fs, fd);		// Le cache doit quand-même être mis à jour pour être en concordance avec current_block_number
			}
		}
		else
		{
			if (fd->current_block_number + 1 >= BLOCKS_NUMBER)
			{
				/* On laisse l'offset en EOF car on est à la fin du fichier et il est impossible de poursuivre l'écriture */
				return bytes_written;
			}
			else if (fd->inode->blocks_index[fd->current_block_number + 1] == 0)
			{
				/* Il est possible de poursuivre l'écriture mais il faut allouer un nouveau bloc */
				if ((fd->inode->blocks_index[fd->current_block_number + 1] = allocateBlock(fs) + 1) == 0)
				{
					fprintf(stderr, "Erreur dans dwrite : Impossible d'allouer un nouveau bloc de données\n");
					return bytes_written;
				}
				/* Allocation réussie, écriture au tour suivant de boucle */
			}
			else
			{
				/* On peut directement basculer sur le bloc suivant qui est alloué */
				fd->current_block_number++;
				fd->offset = 0;
				updateCache(fs, fd);		// Le cache doit quand-même être mis à jour pour être en concordance avec current_block_number
			}
		}
	}
	return bytes_written;
}

void dreadFullBlock(uint8_t *dest, FileSystem *fs, Descriptor* fd, uint8_t block_number)
{
	if (block_number < BLOCKS_NUMBER && fd->inode->blocks_index[block_number] != 0)
	{
		if (fd->current_block_number == block_number)
		{
			memcpy(dest, fd->cache.content, BLOCK_SIZE);
		}
		else
		{
			memcpy(dest, getBlockAddress(fs, fd->inode->blocks_index[block_number] - 1), BLOCK_SIZE);
		}
	}
	else
	{
		fprintf(stderr, "Erreur dans dreadFullBlock : block_number invalide.");
	}
}

void dwriteFullBlock(uint8_t *src, FileSystem *fs, Descriptor* fd, uint8_t block_number)
{
	if (block_number < BLOCKS_NUMBER)
	{
		if (fd->inode->blocks_index[block_number] == 0)
		{
			if (fd->inode->blocks_index[block_number - 1] == 0)
			{
				fprintf(stderr, "Erreur dans dwriteFullBlock : block_number invalide.");
				return;
			}
			else
			{
				if ((fd->inode->blocks_index[block_number] = allocateBlock(fs) + 1) == 0)
				{
					fprintf(stderr, "Erreur dans dwriteFullBlock : Echec d'allocation d'un bloc.\n");
					return;
				}
				fd->inode->size += 16;
			}
		}
		memcpy(getBlockAddress(fs, fd->inode->blocks_index[block_number] - 1), src, BLOCK_SIZE);
		if (block_number == fd->current_block_number)
		{
			updateCache(fs, fd);		// Le cache doit quand-même être mis à jour pour être en concordance avec current_block_number
		}	
	}
}
