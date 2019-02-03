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

all : Object_files/test-gc.o Object_files/list.o Object_files/memory.o
	$(COMPILER) $(LFLAGS) $(CFLAGS) Object_files/test-gc.o Object_files/list.o Object_files/memory.o -o executable.bin
	@echo 
	@echo Exécutable généré via $(COMPILER) en mode $(BUILD) $(LIB)


# Règles Compilation
Object_files/test-gc.o : Sources/test-gc.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/test-gc.c -o Object_files/test-gc.o

Object_files/list.o : Sources/list.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/list.c -o Object_files/list.o

Object_files/memory.o : Sources/memory.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/memory.c -o Object_files/memory.o

# Suppression des fichiers objets

clean:
	rm -rf Object_files/*.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable.bin
