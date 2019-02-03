#include "sys_fic.h"
#include "descripteur.h"
#include "appel_sys.h"

int createFile(FileSystem *fs, char *name)
{
	int i = 0;
	int inode_index = 0;
	Inode *inode = NULL;

	if ((inode_index = allocateInode(fs)) == -1)
	{
		fprintf(stderr, "Erreur dans createFile : Echec de l'allocation d'un inode.\n");
		return -1;
	}
	else
	{
		inode = &fs->inodes_array[inode_index];
		if (strlen(name) > FILE_NAME_SIZE)
		{
			inode->name_size = FILE_NAME_SIZE;
		}
		else
		{
			inode->name_size = strlen(name);
		}
		strncpy(inode->name, name, inode->name_size);
		inode->size = 0;
		if ((inode->blocks_index[0] = allocateBlock(fs) + 1) == 0)
		{
			fprintf(stderr, "Erreur dans createFile : Echec de l'allocation d'un bloc de données.\n");
			return -1;
		}
		for (i = 1; i < BLOCKS_NUMBER; i++)
		{
			inode->blocks_index[i] = 0;
		}
		return inode_index;
	}
}

Descriptor* openFile(FileSystem *fs, char *name)
{
	int i = 0;
	if (strlen(name) > 8)
	{
		fprintf(stderr, "Erreur dans openFile : La taille du nom du fichier demandée est trop longue.\n");
		return NULL;
	}
	else
	{
		for (i = 0; i < fs->inodes_number; i++)
		{
			if(getUsedFlagInode(fs, i) && getFileNameSize(fs, i) == strlen(name) && strncmp(getFileName(fs, i), name, strlen(name)) == 0)
			{
				return initDesciptor(fs, i);
			}
		}
		fprintf(stderr, "Erreur dans openFile : Aucun fichier d'un tel nom trouvé\nPensez à vérifier l'orthographe...\n");
		return NULL;
	}
}
	
void seekInFile(FileSystem *fs, Descriptor* fd, off_t offset, int whence)
{
	dseek(fs, fd, offset, whence);
}

size_t readFile(FileSystem *fs, Descriptor *fd, uint8_t *dest, size_t length)
{
	return dread(fs, fd, dest, length);
}

size_t writeFile(FileSystem *fs, Descriptor *fd, uint8_t *src, size_t length)
{
	return dwrite(fs, fd, src, length);
}

void closeFile(Descriptor *fd)
{
	freeDescriptor(fd);
}
