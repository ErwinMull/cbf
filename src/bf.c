/* ================================ INCLUDES ================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* =============================== CONSTANTS ================================ */

#define TAPE_SIZE 30000
#define LOOP_STACK_SIZE 1024

/* ================================== EVAL ================================== */

void bf_eval(uint8_t *tape, size_t ptr, char *instructions, size_t instruction_len)
{
	size_t loop_stack[LOOP_STACK_SIZE];
	int loop_stack_ptr = -1;
	size_t instruction_ptr = 0;

	while (instruction_ptr < instruction_len) {
		switch(instructions[instruction_ptr]) {
		case '+':
			++tape[ptr];
			break;
		case '-':
			--tape[ptr];
			break;
		case '>':
			if (TAPE_SIZE - 1 == ptr) {
				ptr = 0;
			} else {
				++ptr;
			}
			break;
		case '<':
			if (0 == ptr) {
				ptr = TAPE_SIZE - 1;
			} else {
				--ptr;
			}
			break;
		case '.':
			putc(tape[ptr], stdout);
			break;
		case ',':
			int c = getc(stdin);
			if (EOF != c) {
				tape[ptr] = (uint8_t)c;
			}
			break;
		case '[':
			if (LOOP_STACK_SIZE - 1 == loop_stack_ptr) {
				// TODO: more sophisticated handling
				return;
			} else if (tape[ptr] == 0) {
				int depth = 1;
				while (depth && ++instruction_ptr < instruction_len) {
					if (instructions[instruction_ptr] == '[') {
						++depth;
					}
					else if (instructions[instruction_ptr] == ']') {
						--depth;
					}
				}
			} else {
				loop_stack[++loop_stack_ptr] = instruction_ptr;
			}
			break;
		case ']':
			if (-1 == loop_stack_ptr) {
				// TODO: more sophisticated handling
				return;
			} else if (0 != tape[ptr]) {
				instruction_ptr = loop_stack[loop_stack_ptr];
			} else {
				--loop_stack_ptr;
			}
			break;
		}
		++instruction_ptr;
	}
}

/* ================================== MAIN ================================== */

int main(int argc, char **argv) {
	FILE *fp;
	size_t size;
	char *buf;

	uint8_t tape[TAPE_SIZE] = {0};

	if (argc != 2) {
		fprintf(stderr, "Missing file parameter\n");
		return EXIT_FAILURE;
	}

	fp = fopen(argv[1], "r");
	if (!fp) {
		fprintf(stderr, "Could not open file '%s'\n", argv[1]);
		return EXIT_FAILURE;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	buf = malloc(size);
	if (!buf) {
		fprintf(stderr, "Could not allocate buffer\n");
		return EXIT_FAILURE;
	}
	if (size != fread(buf, sizeof(char), size, fp)) {
		fprintf(stderr, "Error reading the file\n");
		return EXIT_FAILURE;
	}
	fclose(fp);

	bf_eval(tape, 0, buf, size);

	free(buf);

	return 0;
}
