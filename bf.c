/*** INCLUDES ***/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*** DEFINES ***/

#define DATA_SIZE 30000
#define INSTRUCTIONS_SIZE 2048

/*** ERROR HANDLING ***/

void die(const char *s) {
  perror(s);
  exit(1);
}

/*** DATA ***/

unsigned char data[30000];
int data_pointer = 0;

void incrementByte() {
  data[data_pointer]++;
}

void decrementByte() {
  data[data_pointer]--;
}

void incrementPointer() {
  data_pointer++;
}

void decrementPointer() {
  data_pointer--;
}

void outputByte() {
  putc(data[data_pointer], stdout);
}

void inputByte() {
  data[data_pointer] = getc(stdin);
}

unsigned char peekByte() {
  return data[data_pointer];
}

void evalInstructions(char *i, int size) {
  int ip = 0;
  int bc = 0;
  
  while (ip < size) {
    switch (i[ip]) {
      case '<':
        decrementPointer();
        break;
      case '>':
        incrementPointer();
        break;
      case '-':
        decrementByte();
        break;
      case '+':
        incrementByte();
        break;
      case '.':
        outputByte();
        break;
      case ',':
        inputByte();
        break;
      case '[':
        bc++;
        if (peekByte() == 0) {
          int exitBc = bc;
          while (1) {
            ip++;
            if (ip == size) die("Unmatched opening bracket!");
            if (i[ip] == '[') {
              bc++;
            } else if (i[ip] == ']') {
              if (bc == exitBc) break;
              bc--;
            }
          }
          bc--;
        }
        break;
      case ']':
        if (peekByte() != 0) {
          int exitBc = bc;
          while (1) {
            ip--;
            if (ip == 0) die("Unmatched closing bracket!");
            if (i[ip] == ']') {
              bc++;
            } else if (i[ip] == '[') {
              if (bc == exitBc) break;
              bc--;
            }
          }
        } else {
          bc--;
        }
        break;
      default:
        die("Encountered invalid token!");
    }
    ip++;
  }
  if (bc != 0) die("Unmatched opening bracket!");
}
  
int main(int argc, char *argv[]) {
  if (argc != 2)
    die("Invalid number of input arguments!");
  
  int i;
  for (i = 0; i < DATA_SIZE; i++) 
    data[i] = 0;

  FILE *fp = fopen(argv[1], "r");
  if (!fp) die("File could not be opened!");

  char instructions[INSTRUCTIONS_SIZE + 1];
  char c;
  i = 0;
  while ((c = fgetc(fp)) != EOF) {
    if (i >= INSTRUCTIONS_SIZE) die("Too many instructions!");
    if (c == '<' || c == '>' || c == '+' || c == '-' ||
        c == '.' || c == ',' || c == '[' || c == ']')
      instructions[i++] = c;
  }

  evalInstructions(instructions, i);
  
  return 0;
}
