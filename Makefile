CC = gcc
CFLAGS = -Wall -std=gnu99 -O3 -fstrict-aliasing
EXEC_NAME = exec
INCLUDES =
LIBS =
FILES =  $(wildcard *.c)
OBJ_FILES = $(FILES:.c=.o)
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm -f $(EXEC_NAME) $(OBJ_FILES) .depends *~ *#

$(EXEC_NAME) : $(OBJ_FILES) $(FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS) -lgomp

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $< -fopenmp

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)

.depends:
	$(CC) -MM $(FILES) > .depends

-include .depends
