#ifndef sys_call_H_INCLUDED
#define sys_call_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* DEFINES */

#define TRUE 1
#define FALSE 0

#define SEEK_SET 0		/* La tête est placée au début du fichier */
#define SEEK_CUR 1		/* La tête conserve la position courante dans le fichier */
#define SEEK_END 2		/* La tête est placée à la fin du fichier */

/* PROTOTYPES DES FONCTIONS */

int createFile(FileSystem *fs, char *name);
/* Créé un fichier dans le système de fichiers. Retourne l'index de l'inode en cas de réussite
compris entre 0 et "fs->inodes_number".
Retourne -1 en cas d'échec. */

Descriptor* openFile(FileSystem *fs, char *name);
/* Ouvre le fichier dont le nom est "name" dans le système de fichiers "fs". Ne pas oublier de le fermer avec la fonction closeFile par la suite.
La fonction retourne un pointeur sur le descriptor de fichier nouvellement alloué pour le fichier demandé. */

void seekInFile(FileSystem *fs, Descriptor* fd, off_t offset, int whence);
/* Change le décalage courant ("offset") et/ou le bloc courant ("current_block_number") dans le descipteur de fichiers "fd".
"whence" détermine la nouvelle position de la tête. Ce paramètre pêut prendre trois valeurs :
SEEK_SET : la tête est placée au début du fichier
SEEK_CUR : La tête conserve la position courante dans le fichier
SEEK_END : La tête est placée à la fin du fichier
"offset" détermine le décalage (positif ou négatif) à partir de la nouvelle position de la tête. */

size_t readFile(FileSystem *fs, Descriptor *fd, uint8_t *dest, size_t length);
/* Lis des octets depuis de le fichier dont le descipteur est "fd", dans le système de fichiers "fs".
"length" détermine le nombre d'octets à lire.
"dest" est le tableau de destination.
/!\ Aucune allocation dynamique n'est faite dans cette fonction. */

size_t writeFile(FileSystem *fs, Descriptor *fd, uint8_t *src, size_t length);
/* Ecris des octets dans le fichier dont le descipteur est "fd", dans le système de fichiers "fs".
"length" détermine le nombre d'octets à écrire.
"src" est le tableau source. */

void closeFile(Descriptor *fd);
/* Ferme le fichier dont le descriptor est "fd".
La mémoire allouée pour le descriptor dans openFile est libérée dans cette fonction. */


#endif
