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

all : Object_files/main.o Object_files/file_sys.o Object_files/descriptor.o Object_files/sys_call.o
	$(COMPILER) $(LFLAGS) $(CFLAGS) Object_files/main.o Object_files/file_sys.o Object_files/descriptor.o Object_files/sys_call.o -o executable.bin -lm
	@echo 
	@echo Exécutable généré via $(COMPILER) en mode $(BUILD) $(LIB)


# Règles Compilation

Object_files/main.o : Sources/main.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/main.c -o Object_files/main.o

Object_files/file_sys.o : Sources/file_sys.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/file_sys.c -o Object_files/file_sys.o

Object_files/descriptor.o : Sources/descriptor.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/descriptor.c -o Object_files/descriptor.o

Object_files/sys_call.o : Sources/sys_call.c
	$(COMPILER) $(LFLAGS) $(CFLAGS) -I Headers -c Sources/sys_call.c -o Object_files/sys_call.o


# Suppression des fichiers objets

clean:
	rm -rf Object_files/*.o

# Suppression de tous les fichiers, sauf les sources, en vue d'une reconstruction complète

mrproper: clean
	rm -rf executable.bin
