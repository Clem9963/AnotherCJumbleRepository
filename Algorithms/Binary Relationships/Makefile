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

all : Object_files/ex1.o Object_files/ex2.o Object_files/ex3.o Object_files/ex4.o Object_files/test.o
	$(COMPILER) $(LFLAGS) $(CFLAGS) Object_files/ex1.o Object_files/ex2.o Object_files/ex3.o Object_files/ex4.o Object_files/test.o -o executable.bin
	@echo 
	@echo Exécutable généré via $(COMPILER) en mode $(BUILD) $(LIB)


# Règles Compilation

Object_files/ex1.o : Sources/ex1.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/ex1.c -o Object_files/ex1.o

Object_files/ex2.o : Sources/ex2.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/ex2.c -o Object_files/ex2.o
 
Object_files/ex3.o : Sources/ex3.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/ex3.c -o Object_files/ex3.o

Object_files/ex4.o : Sources/ex4.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/ex4.c -o Object_files/ex4.o

Object_files/test.o : Sources/test.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/test.c -o Object_files/test.o


# Suppression des fichiers objets

clean:
	rm -rf Object_files/*.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable.bin