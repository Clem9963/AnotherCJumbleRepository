all : main.c
	gcc -Wall -Wextra main.c -o executable

clean:
	rm -rf *.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable