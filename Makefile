CC=gcc
FLAG = -Wall -g3 -Wextra -MMD -Werror
HEAD = ./include
OBJ = $(addprefix obj/, my_parser.o my_calc.o)
D_LST =  $(addprefix obj/, my_parser.d, my_calc.d)
NAME = my_calc

all : $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAG) -I $(HEAD) $(OBJ) -o $(NAME)

obj/%.o: src/%.c | object
	$(CC) $(FLAG) -I $(HEAD) -c $< -o $@

object:
	@mkdir -p obj

clean :
	rm -f $(NAME)
	$(RM) *.o .*.swp .*.swo

fclean :
	rm -f $(NAME)
	rm -rf obj
	$(RM) *.o .*.swp .*.swo

re : fclean all

.PHONY: all re clean fclean 
-include $(D_LST)
