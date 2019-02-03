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

all : Object_files/main.o Object_files/sys_fic.o Object_files/descripteur.o Object_files/appel_sys.o
	$(COMPILER) $(LFLAGS) $(CFLAGS) Object_files/main.o Object_files/sys_fic.o Object_files/descripteur.o Object_files/appel_sys.o -o executable.bin -lm
	@echo 
	@echo Exécutable généré via $(COMPILER) en mode $(BUILD) $(LIB)


# Règles Compilation

Object_files/main.o : Sources/main.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/main.c -o Object_files/main.o

Object_files/sys_fic.o : Sources/sys_fic.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/sys_fic.c -o Object_files/sys_fic.o

Object_files/descripteur.o : Sources/descripteur.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/descripteur.c -o Object_files/descripteur.o

Object_files/appel_sys.o : Sources/appel_sys.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/appel_sys.c -o Object_files/appel_sys.o


# Suppression des fichiers objets

clean:
	rm -rf Object_files/*.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable.bin
