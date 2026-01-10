CC := gcc
FLAGS := -Wall -Wextra

NAME := bf

${NAME}.out: ${NAME}.c
	${CC} $^ -o $@ ${FLAGS}
