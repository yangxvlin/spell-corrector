# # # # # # #
# Makefile for assignment 2
#
# created May 2018
# Matt Farrugia <matt.farrugia@unimelb.edu.au>
#

CC     = gcc
CFLAGS = -Wall -std=c99 -lm -O2
# modify the flags here ^
EXE    = a2
OBJ    = main.o list.o spell.o hash.o hashtbl.o print.o distance.o search.o strarr.o
# add any new object files here ^

# top (default) target
all: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# other dependencies
main.o: list.h spell.h
spell.o: spell.h list.h print.h hashtbl.h distance.h search.h strarr.h
list.o: list.h
print.o: print.h
strarr.o: strarr.h
distance.o: distance.h
hash.o: hash.h
hashtbl.o: hashtbl.h hash.h
search.o: list.h hashtbl.h distance.h strarr.h
# ^ add any new dependencies here (for example if you add new modules)


# phony targets (these targets do not represent actual files)
.PHONY: clean cleanly all CLEAN

# `make clean` to remove all object files
# `make CLEAN` to remove all object and executable files
# `make cleanly` to `make` then immediately remove object files (inefficient)
clean:
	rm -f $(OBJ)
CLEAN: clean
	rm -f $(EXE)
cleanly: all clean
