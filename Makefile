NAME	= push_swap

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I .

SRC		= bench.c convert_input.c parse_flags.c input_utils.c list_init.c sort_complex.c sort_operations.c sort_operations2.c sort_operations3.c sort_simple.c sort_small.c compute_disorder.c sort_adaptive.c sort_medium_utils.c sort_medium.c \
		  sort_low.c sort_low_cost.c sort_low_lis.c \
		  sort_complex_utils.c sort_complex_base.c

OBJ		= $(SRC:.c=.o)

RM		= rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re