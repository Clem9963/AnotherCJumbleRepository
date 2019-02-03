#ifndef SYST_FIC_H_INCLUDED
#define SYST_FIC_H_INCLUDED

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_NAME_SIZE 8
#define BLOCKS_NUMBER 6
#define BLOCK_SIZE 16

/* STRUCTURES DE DONNEES */

struct Inode
{
	uint8_t name_size;
	char name[FILE_NAME_SIZE];
	uint8_t size;
	uint8_t blocks_index[BLOCKS_NUMBER];
};
typedef struct Inode Inode;

struct Block
{
	uint8_t content[BLOCK_SIZE];
};
typedef struct Block Block;

struct FileSystem
{
	uint8_t blocks_number;
	uint8_t inodes_number;
	uint8_t *blocks_bitmap;
	uint8_t *inodes_bitmap;
	Inode *inodes_array;
	Block *blocks_array;
};

typedef struct FileSystem FileSystem;

/* DEFINES */

#define TRUE 1
#define FALSE 0

/* PROTOTYPES DES FONCTIONS */

FileSystem* initFileSystem();
/* Alloue la mémoire pour une strucure représentant un sytème de fichiers.
Retourne un pointeur sur le nouveau système de fichiers alloué. */

void freeFileSystem(FileSystem *fs);
/* Libère la mémoire pour une strucure représentant un sytème de fichiers ainsi que celle
utilisée par cette dernière. */

int loadFileSystem(FileSystem *fs, char *path);
/* Charge un système de fichiers depuis le chemin sur le disque "path" dans "fs". */

void saveFileSystem(FileSystem *fs, char *path);
/* Sauvegarde le système de fichiers "fs" dans le chemin sur le disque "path". */

int allocateBlock(FileSystem *fs);
/* Alloue un bloc de données pour le contenu d'un fichier.
Retourne l'index du bloc en cas de succès entre 0 inclus et "fs->blocks_number" exclu.
Retourne -1 en cas d'échec. */

int allocateInode(FileSystem *fs);
/* Alloue un inode pour un fichier.
Retourne l'index de l'inode en cas de succès entre 0 inclus et "fs->inodes_number" exclu.
Retourne -1 en cas d'échec. */

int getUsedInodesNumber(FileSystem *fs);
/* Renvoie le nombre d'inodes utilisés dans le système de fichiers "fs". */

int getUsedFlagInode(FileSystem *fs, int inode_index);
/* Retourne TRUE si l'inode d'indice "inode_index" est utilisé dans le système de fichiers "fs".
Retourne FALSE sinon. A noter que, si l'indice est erroné, retourne TRUE par sécurité. */

void setUsedFlagInode(FileSystem *fs, int inode_index, uint8_t value);
/* Définit la valeur "value" du flag d'utilisation de l'inode d'indice "inode_index" dans le système de fichiers "fs". */

int getUsedFlagBlock(FileSystem *fs, int block_index);
/* Retourne TRUE si le bloc d'indice "block_index" est utilisé dans le système de fichiers "fs".
Retourne FALSE sinon. A noter que, si l'indice est erroné, retourne TRUE par sécurité. */

void setUsedFlagBlock(FileSystem *fs, int inode_index, uint8_t value);
/* Définit la valeur "value" du flag d'utilisation du bloc d'indice "block_index" dans le système de fichiers "fs". */

uint8_t getBitInByte(uint8_t byte, uint8_t bit_number);
/* Extrait le bit numéro "bit_number" dans l'octet "byte".
/!\ La numérotation est effectuée en commencant par le bit de poids faible. */

void setBitInByte(uint8_t *byte, uint8_t bit_number, uint8_t value);
/* Définit le bit numéro "bit_number" dans l'octet "byte" à la valeur "value".
/!\ La numérotation est effectuée en commencant par le bit de poids faible. */

char* getFileName(FileSystem *fs, int inode_index);
/* Retourne le nom du fichier dont l'inode est d'indice "inode_index" dans "fs." */

uint8_t getFileNameSize(FileSystem *fs, int inode_index);
/* Retourne la taille du nom du fichier dont l'inode est d'indice "inode_index" dans "fs". */

uint8_t getFileSize(FileSystem *fs, int inode_index);
/* Retourne la taille du fichier dont l'inode est d'indice "inode_index" dans "fs". */

uint8_t* getBlocksIndex(FileSystem *fs, int inode_index);
/* Retourne les indices des blocs de données de l'inode d'indice "inode_index" dans le système de fichiers "fs". */

uint8_t* getBlockAddress(FileSystem *fs, int block_index);
/* Permet de lire le bloc d'indice "block_index" dans le système de fichiers "fs".
Retourne l'adresse mémoire du contenu du bloc si l'indice est valide, le pointeur NULL sinon. */

void displayBlockContent(uint8_t *content);
/* Affiche le contenu du tableau "content" de 16 d'octets en formatage caractère. */


#endif
