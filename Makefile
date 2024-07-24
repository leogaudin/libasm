GREEN = \033[0;32m
RED = \033[0;31m
NC = \033[0m

NAME = libasm.a

SOURCES = ft_strlen.s \
		  ft_strcpy.s \
		  ft_strcmp.s \
		  ft_write.s \
		  ft_read.s \
		#   ft_strdup.s

OBJECTS = $(SOURCES:.s=.o)

NASM = nasm
NASMFLAGS = -f macho64

CC = gcc

ifeq ($(shell uname -m), arm64)
	CC += -ld_classic --target=x86_64-apple-darwin
endif

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	@ar rc $(NAME) $(OBJECTS)
	@echo "${GREEN}+ $(NAME)${NC}"

%.o: %.s
	@$(NASM) $(NASMFLAGS) $< -o $@

clean:
	@rm -f $(OBJECTS)
	@echo "${RED}- $(OBJECTS)${NC}"

fclean: clean
	@rm -f $(NAME)
	@rm -f test
	@echo "${RED}- $(NAME)${NC}"

re: fclean all

test: all
	@$(CC) $(CFLAGS) $(NAME) main.c -o test && ./test && rm -f test

.PHONY: all clean fclean re
