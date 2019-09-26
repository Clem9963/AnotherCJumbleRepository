#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct nombreA2Chiffres
{
	char dizaines;
	char unites;
};

struct nombreA2Chiffres arith(char x, char y, int bool);
struct nombreA2Chiffres somme2chiffres(char* x, char* y, int longx, int longy);
struct nombreA2Chiffres pdt(char* x, char* y, int longx, int longy);
void afficherNombre(struct nombreA2Chiffres nb);

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		fprintf(stderr, "Arguments fournis incorrects\n");
		exit(EXIT_FAILURE);
	}

	if (!strcmp(argv[1], "-a"))
	{
		printf("%d\n", atoi(argv[2]) + atoi(argv[3]));
	}

	if (!strcmp(argv[1], "-m"))
	{
		printf("%d\n", atoi(argv[2]) * atoi(argv[3]));
	}

	return EXIT_SUCCESS;
}

struct nombreA2Chiffres arith(char x, char y, int b)
{
	struct nombreA2Chiffres nb;
	char temp;
	if (x < 0 || x > 9 || y < 0 || y > 9)
	{
		fprintf(stderr, "Arguments incorrects\n");
		nb.unites = 0;
		nb.dizaines = 0;
		return nb;
	}
	if (b)
	{
		temp = x*y;
		nb.unites = temp % 10;
		nb.dizaines = temp / 10 % 10;
	}
	else
	{
		temp = x+y;
		nb.unites = temp % 10;
		nb.dizaines = temp / 10 % 10;
	}
	return nb;
}

struct nombreA2Chiffres somme2chiffres(char* x, char* y, int longx, int longy)
{
	struct nombreA2Chiffres nb;
	nb = arith(x[0], y[0], FALSE);
	if (longx == 1)
	{
		if (longy != 1)
		{
			nb.dizaines = (nb.dizaines + y[1]) % 10;
		}
	}
	else
	{
		if (longy == 1)
		{
			nb.dizaines = (nb.dizaines + x[1]) % 10;
		}
		else
		{
			nb.dizaines = (nb.dizaines + x[1] + y[1]) % 10;
		}
	}

	return nb;
}

struct nombreA2Chiffres pdt(char* x, char* y, int longx, int longy)
{
	struct nombreA2Chiffres nb;
	nb = arith(x[0], y[0], TRUE);
	if (longx == 1)
	{
		if (longy != 1)
		{
			nb.dizaines = (nb.dizaines + y[1] * x[0]) % 10;
		}
	}
	else
	{
		if (longy == 1)
		{
			nb.dizaines = (nb.dizaines + x[1] * y[0]) % 10;
		}
		else
		{
			nb.dizaines = (nb.dizaines + x[1] * y[0] + x[0] * y[1]) % 10;
		}
	}

	return nb;
}

void afficherNombre(struct nombreA2Chiffres nb)
{
	printf("%d%d\n", nb.dizaines, nb.unites);
}