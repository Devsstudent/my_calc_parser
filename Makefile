CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -std=c99 -I./include -I./include/criterion -pedantic
OBJ = my_parser.o my_calc.o test.o
C_FILE = my_parser.c my_calc.c test.c
NAME = my_calc

all: 
	$(CC) $(CFLAGS) $(C_FILE) -o $(NAME)

test: ${OBJ}
	$(CC) $(CFLAGS) -L ./lib -lcriterion -o test $(OBJ)
	timeout 1m ./test

clean:
	$(RM) *.o .*.swp .*.swo

distclean: clean
	$(RM) test

.PHONY: all clean distclean test
