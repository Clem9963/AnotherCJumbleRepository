#ifndef descriptor_H_INCLUDED
#define descriptor_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* STRUCTURES DE DONNEES */

struct Descriptor
{
	Inode *inode;
	uint8_t current_block_number;
	off_t offset;
	Block cache;
};
typedef struct Descriptor Descriptor;

/* DEFINES */

#define TRUE 1
#define FALSE 0

#define SEEK_SET 0		/* La tête est placée au début du fichier */
#define SEEK_CUR 1		/* La tête conserve la position courante dans le fichier */
#define SEEK_END 2		/* La tête est placée à la fin du fichier */

/* PROTOTYPES DES FONCTIONS */

Descriptor* initDesciptor(FileSystem *fs, int inode_index);
/* Retourne un descriptor alloué dynamiquement pour le fichier pointé par l'inode d'indice "inode_index" dans "fs" */

void freeDescriptor(Descriptor *fd);
/* Libère toute la mémoire allouée pour le descriptor "fd". */

void updateCache(FileSystem *fs, Descriptor* fd);
/* Met à jour le cache du descriptor de fichiers "fd" en fonction du numéro du bloc courant */

void dseek(FileSystem *fs, Descriptor* fd, off_t offset, int whence);
/* Change le décalage courant ("offset") et/ou le bloc courant ("current_block_number") dans le descipteur de fichiers "fd".
"whence" détermine la nouvelle position de la tête. Ce paramètre pêut prendre trois valeurs :
SEEK_SET : la tête est placée au début du fichier
SEEK_CUR : La tête conserve la position courante dans le fichier
SEEK_END : La tête est placée à la fin du fichier
"offset" détermine le décalage (positif ou négatif) à partir de la nouvelle position de la tête. */

size_t dread(FileSystem *fs, Descriptor *fd, uint8_t *dest, size_t length);
/* Lis des octets depuis le fichier dont le descipteur est "fd", dans le système de fichiers "fs".
"length" détermine le nombre d'octets à lire.
"dest" est le tableau de destination.
/!\ Aucune allocation dynamique n'est faite dans cette fonction. */

size_t dwrite(FileSystem *fs, Descriptor *fd, uint8_t *src, size_t length);
/* Ecris des octets dans le fichier dont le descipteur est "fd", dans le système de fichiers "fs".
"length" détermine le nombre d'octets à écrire.
"src" est le tableau source. */

/* LES FONCTIONS SUIVANTES NE DOIVENT ETRE UTILISEES QUE POUR LE DEBUG */

void dreadFullBlock(uint8_t *dest, FileSystem *fs, Descriptor* fd, uint8_t block_number);
/* Lis le bloc numéro "block_number" du fichier dont le descriptor est "fd", dans le système de fichiers "fs".
La destination est "dest". A noter que cette fonction sert surtout pour le debug !
/!\ Aucune allocation dynamique n'est faite dans cette fonction. */

void dwriteFullBlock(uint8_t *src, FileSystem *fs, Descriptor* fd, uint8_t block_number);
/* Ecris les octets du tableau "src" dans le bloc numéro "block_number" du fichier dont le descriptor est "fd", dans le système de fichiers "fs".
A noter que cette fonction sert surtout pour le debug ! */


#endif
