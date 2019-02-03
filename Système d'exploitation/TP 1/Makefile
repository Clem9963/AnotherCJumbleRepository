# Choix des arguments pour le compilateur

COMPILER=gcc
BUILD=debug
LIB=shared

ifeq ($(BUILD),debug)
	CFLAGS=-Wall -Wextra -g
endif

ifeq ($(LIB),static)
	LFLAGS=-static
endif

# Règles Linkage

all : Object_files/main.o
	$(COMPILER) $(LFLAGS) $(CFLAGS) Object_files/main.o -o executable.bin
	@echo 
	@echo Exécutable généré via $(COMPILER) en mode $(BUILD) $(LIB)


# Règles Compilation

Object_files/main.o : Sources/main.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/main.c -o Object_files/main.o

# Suppression des fichiers objets

clean:
	rm -rf Object_files/*.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable.bin