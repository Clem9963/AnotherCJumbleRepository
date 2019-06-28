#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

void lire_vecteur_a();
off_t rembobiner(int descripteur_fichier);
size_t lire_longueur_fichier(int descripteur_fichier);
void nombre_element_vecteur(int fichier_vec, size_t *longueur_fichier, unsigned int *n_elements);
void lire_fichier_vec(int fichier_vec, double **vecteur, unsigned int *n_elements);

int main()
{
	char pathname[16] = "./vecteur_a.vec";
	int fd;

	fd = open(pathname, O_RDONLY);

	size_t longueur_fichier;
	unsigned int n_elements;
	nombre_element_vecteur(fd, &longueur_fichier, &n_elements);
	printf("%lu %d\n", longueur_fichier, n_elements);

	return EXIT_SUCCESS;
}

void lire_vecteur_a()
{
	char pathname[16] = "./vecteur_a.vec";
	char buffer;
	int fd;
	int continuer = TRUE;

	fd = open(pathname, O_RDONLY);
	if (fd == -1)
	{
		exit(EXIT_FAILURE);
	}

	while(continuer)
	{
		switch (read(fd, &buffer, 1))
		{
			case(-1):
				exit(EXIT_FAILURE);
				break;
			case(0):
				continuer = FALSE;
				break;
			default:
				printf("%c", buffer);
				break;
		}
	}
	printf("\n");
}

off_t rembobiner(int descripteur_fichier)
{
	return lseek(descripteur_fichier, 0, SEEK_SET);
}

size_t lire_longueur_fichier(int descripteur_fichier)
{
	off_t taille;
	off_t ancienne_position = lseek(descripteur_fichier, 0, SEEK_CUR);
	taille = lseek(descripteur_fichier, 0, SEEK_END);
	lseek(descripteur_fichier, ancienne_position, SEEK_SET);

	return (size_t)taille;
}

void nombre_element_vecteur(int fichier_vec, size_t *longueur_fichier, unsigned int *n_elements)
{
	int continuer = TRUE;
	int nb_virgules = 0;
	char buffer;

	*longueur_fichier = lire_longueur_fichier(fichier_vec);

	while(continuer)
	{
		switch (read(fichier_vec, &buffer, 1))
		{
			case(-1):
				exit(EXIT_FAILURE);
				break;
			case(0):
				continuer = FALSE;
				break;
			default:
				if (buffer == ',')
				{
					nb_virgules++;
				}
				break;
		}
	}

	*n_elements = nb_virgules + 1;
}

void lire_fichier_vec(int fichier_vec, double **vecteur, unsigned int *n_elements)
{
	size_t longueur_fichier;
	char buffer_char;
	char buffer_str[256];
	int continuer = TRUE;
	int indice_matrice = 0;
	int indice = 0;
	nombre_element_vecteur(fichier_vec, &longueur_fichier, n_elements);

	*vecteur = calloc(*n_elements, sizeof(double));

	while(continuer)
	{
		if (indice == -1)
		{
			*vecteur[indice_matrice] = strtod(buffer_str, NULL);
		}

		switch (read(fichier_vec, &buffer_char, 1))
		{
			case(-1):
				exit(EXIT_FAILURE);
				break;
			case(0):
				continuer = FALSE;
				break;
			default:
				if (buffer_char == '\n')
				{
					return;
				}
				else if (buffer_char == ',')
				{
					buffer_str[indice] = '\0';
					indice = -1;
				}
				else
				{
					buffer_str[indice] = buffer_char;
					indice++;
				}
				break;
		}
	}
}