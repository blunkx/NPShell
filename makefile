# npshell: npshell.cc
# 	g++ -o ./npshell npshell.cc

# clean:
# 	rm -f *.o npshell
CURRENT_DIR = $(shell pwd)
DIR_INC = ./include
DIR_SRC = ./source
DIR_OBJ = ./obj

SRC = $(wildcard $(DIR_SRC)/*.cc)
OBJ = $(patsubst %.cc, ${DIR_OBJ}/%.o, $(notdir $(SRC)))

CC = g++
CFLAGS = -g -Wall -I$(DIR_INC) -std=c++20

# $@  all target
# $^  all dependencies
# $<  first dependency
# $?  all dependencies newer than target
# TARGET = 

TARGET = npshell

${TARGET}:$(OBJ)
	$(CC) -o $@ $^

$(DIR_OBJ)/%.o: ${DIR_SRC}/%.cc
	mkdir -p $(DIR_OBJ)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*.o -f npshell