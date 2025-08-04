NAME        = philo
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -I. #-g -fsanitize=thread

SRCDIR      = .

GREEN       = \033[0;32m
BLUE        = \033[0;36m
YELLOW      = \033[0;33m
RED         = \033[0;31m
RESET       = \033[0m


SRCS        = destroy_philo.c \
			  philo_routine.c \
			  philo_actions.c \
			  philosophers_utils.c \
			  monitor_philos.c \
			  mutex_helpers.c \
			  start_simulation.c \
			  main.c

OBJS        = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ Compilation successful: $(NAME)$(RESET)"

clean:
	@rm -f *.o
	@echo "$(BLUE)🧹 Object files cleaned.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🧨 Executable '$(NAME)' removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
