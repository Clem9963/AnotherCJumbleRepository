#include <stdio.h>
#include <stdlib.h> // Pour malloc
#include <string.h>

typedef enum
{
	ROUGE, BLEU, VERT
} Couleur;

struct poupee_russe
{
	unsigned int taille;
	Couleur couleur;
	char *prenom;
	struct poupee_russe *test;
};
typedef struct poupee_russe poupee_russe;

/*PROTOTYPES */
poupee_russe *cree_poupee(int taille, Couleur couleur);
poupee_russe *cree_poupee_nommee(int taille, Couleur couleur, char prenom[]);
void detruit_poupee(poupee_russe *ptr_poupee);
unsigned int get_taille(poupee_russe *p);
void set_taille(poupee_russe *p, unsigned int taille);
unsigned int get_couleur(poupee_russe *p);
void set_couleur(poupee_russe *p, Couleur couleur);
char* get_prenom(poupee_russe *p);
void set_prenom(poupee_russe *p, char *prenom);

int main()
{
	return 0;
}

poupee_russe *cree_poupee(int taille, Couleur couleur)
{
	poupee_russe *ptr_poupee = malloc(sizeof(poupee_russe));
	ptr_poupee -> taille = taille;
	ptr_poupee -> couleur = couleur;
	ptr_poupee -> prenom = NULL;
	return ptr_poupee;
}

poupee_russe *cree_poupee_nommee(int taille, Couleur couleur, char prenom[])
{
	poupee_russe *ptr_poupee = malloc(sizeof(poupee_russe));
	ptr_poupee -> taille = taille;
	ptr_poupee -> couleur = couleur;
	ptr_poupee -> prenom = malloc((strlen(prenom)+1)*sizeof(char));
	strcpy(ptr_poupee -> prenom, prenom);

	return ptr_poupee;
}

void detruit_poupee(poupee_russe *ptr_poupee)
{
	if (ptr_poupee -> prenom != NULL)
	{
		free(ptr_poupee -> prenom);
	}
	free(ptr_poupee);
}

unsigned int get_taille(poupee_russe *p)
{
	return p -> taille;
}

void set_taille(poupee_russe *p, unsigned int taille)
{
	p -> taille = taille;
}

Couleur get_couleur(poupee_russe *p)
{
	return p -> couleur;
}

void set_couleur(poupee_russe *p, Couleur couleur)
{
	p -> couleur = couleur;
}

char* get_prenom(poupee_russe *p)
{
	return p -> prenom;
}

void set_prenom(poupee_russe *p, char *prenom)
{
	p -> prenom = realloc(p -> prenom, (strlen(prenom)+1)*sizeof(char));
	strcpy(p -> prenom, prenom);
}