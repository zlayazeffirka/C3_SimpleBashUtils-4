#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
struct Flags {
  int n;
  int b;
  int s;
  int e;
  int t;
  int v;
};
void print_symb(int *c, int *prev_c, struct Flags flags, int *index,
                FILE *myfile);
char output_s(char symbol);

int main(int argc, char *argv[]) {
  FILE *myfile;
  struct Flags flags = {0};
  const char *short_options = "sbnvetET";
  const struct option long_options[] = {
      {"squeeze-blank", no_argument, NULL, 's'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {NULL, 0, NULL, 0}};

  int rez;
  while ((rez = getopt_long(argc, argv, short_options, long_options, 0)) !=
         -1) {
    switch (rez) {
      case 's':
        flags.s = 1;
        break;
      case 'b':
        flags.b = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'e':
        flags.e = 1;
        flags.v = 1;
        break;
      case 't':
        flags.t = 1;
        flags.v = 1;
        break;
      case 'E':
        flags.e = 1;
        break;
      case 'T':
        flags.t = 1;
        break;
      case '?':
        fprintf(stderr, "cat: %c: illegal option -- \n", rez);
        exit(EXIT_FAILURE);
        break;
    };
  }
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      myfile = fopen(argv[i], "r");
      if (myfile != NULL) {
        int index = 0;
        int c = fgetc(myfile);
        int prev = '\n';
        while (c != EOF) {
          print_symb(&c, &prev, flags, &index, myfile);
          c = fgetc(myfile);
          if ((flags.e == 1) && (c == '\n')) printf("$");
        }

        fclose(myfile);
      } else {
        fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
        exit(EXIT_FAILURE);
      }
    }
  }
  return 0;
}

void print_symb(int *c, int *prev_c, struct Flags flags, int *index,
                FILE *myfile) {
  int print = 1;
  if (flags.s == 1) {
    int if_empty = 0;
    while ((*prev_c == '\n' && *c == '\n') && *c != EOF) {
      *c = fgetc(myfile);
      if_empty = 1;
    }
    if (if_empty) {
      if (flags.n == 1 && flags.b == 0) {
        *index += 1;
        printf("%6d  ", *index);
      }
      printf("\n");
    }
  }

  if (flags.n == 1 && flags.b == 0) {
    if (*prev_c == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }
  }
  if (flags.b == 1) {
    if ((*prev_c == '\n' && *c != '\n')) {
      *index += 1;
      printf("%6d\t", *index);
    }
  }

  if ((flags.t) && (flags.v) && (*c == 9)) {
    print = 0;
    printf("^I");
  } else {
    if (flags.v == 1) *c = output_s(*c);
  }
  if (print) printf("%c", *c);
  *prev_c = *c;
}

char output_s(char symbol) {
  if ((symbol == '\n') || (symbol == '\t')) return symbol;
  if (symbol <= 31) {
    printf("^");
    symbol += 64;
  } else if (symbol == 127) {
    printf("^");
    symbol = '?';
  }
  return symbol;
}