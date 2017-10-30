#	Circuit Partitioning Suite
#	Edward Armstrong
#	University of Guelph
#	edward@uoguelph.ca
#	File Created: May 26, 2010
#	File Modified: October 5, 2017

CC  = gcc
INC = ./inc
SRC = ./src
BIN = ./bin
OBJ = ./obj
LIB =
DEBUG = -g
FLAGS =  ${DEBUG} -I./inc -O0 -std=c99

default: main

OBJECTS = Circuit.o BucketList.o Sanchis.o

MAIN = main.o ${OBJECTS}
main: $(addprefix ${OBJ}/, ${MAIN})
	@echo Creating Executable $@
	@echo ${CC} $^ -o ${BIN}/$@ ${LIB} ${FLAGS}
	@${CC} $^ -o ${BIN}/$@ ${LIB} ${FLAGS}

circuitTest: ${OBJ}/circuitTest.o ${OBJ}/Circuit.o
	@echo Creating Executable $@
	@echo ${CC} $^ -o ${BIN}/$@ ${LIB} ${FLAGS}
	@${CC} $^ -o ${BIN}/$@ ${LIB} ${FLAGS}

${OBJ}/circuitTest.o: ${SRC}/circuitTest.c
	@echo -- Creating Object File From Source $@
	@${CC} -c $< -o $@ ${FLAGS}

${OBJ}/main.o: ${SRC}/main.c
	@echo -- Creating Object File From Source $@
	@${CC} -c $< -o $@ ${FLAGS}

${OBJ}/BucketList.o: ${SRC}/BucketList.c ${INC}/BucketList.h
	@echo -- Creating Object File From Source $@
	@${CC} -c $< -o $@ ${FLAGS}

${OBJ}/Circuit.o: ${SRC}/Circuit.c ${INC}/Circuit.h
	@echo -- Creating Object File From Source $@
	@${CC} -c $< -o $@ ${FLAGS}

${OBJ}/Sanchis.o: ${SRC}/Sanchis.c ${INC}/Sanchis.h
	@echo -- Creating Object File From Source $@
	@${CC} -c $< -o $@ ${FLAGS}

clean:
	@echo Cleaning All Files
	@rm ${OBJ}/*.o -f
	@rm ${BIN}/* -f