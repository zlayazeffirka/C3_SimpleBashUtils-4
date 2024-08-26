#include "s21_grep.h"

int find_matches(char *patterns, const char *line, flags opts,
                 const char *filename, int line_number) {
  int ret_code = 0;
  regex_t regex;
  regmatch_t match;

  int regs = 0, res = 0;
  regs |= REG_EXTENDED;
  if (opts.i) {
    regs |= REG_ICASE;
  }
  ret_code = regcomp(&regex, patterns, regs);
  if (ret_code) {
    perror("Failed to compile regex");
    return -1;
  }

  while ((ret_code = regexec(&regex, line, 1, &match, 0)) == 0) {
    if (match.rm_so == -1 || res == 1) {
      break;
    }
    if (opts.l || opts.v || (opts.o && opts.c)) {
      res = 1;
    } else if (opts.o) {
      if (opts.print_filename) printf("%s:", filename);
      if (opts.n) printf("%d:", line_number);
      printf("%.*s\n", (int)(match.rm_eo - match.rm_so), line + match.rm_so);
    } else
      res = 1;
    line += match.rm_eo;
  }
  regfree(&regex);
  return res;
}

int grep(const char *line, char *patterns, const char *filename, flags opts,
         int *match_count, int line_number) {
  int match = find_matches(patterns, line, opts, filename, line_number);
  if (match == -1) return -1;

  if ((match && !opts.v) || (!match && opts.v)) {
    if (opts.c || opts.l) {
      (*match_count)++;
    } else {
      if (opts.print_filename) {
        printf("%s:", filename);
      }
      if (opts.n) {
        printf("%d:", line_number);
      }
      printf("%s", line);
      if (line[strlen(line) - 1] != '\n') {
        printf("\n");
      }
    }
  }
  return 0;
}

void get_patterns_file(const char *filename, char **patterns) {
  FILE *pattern_file = fopen(filename, "r");
  if (pattern_file == NULL) {
    perror(filename);
    free(*patterns);
    exit(1);
  }

  char pattern_line[1024];
  while (fgets(pattern_line, sizeof(pattern_line), pattern_file) != NULL) {
    size_t len = strlen(pattern_line);
    if (len == 1 && pattern_line[0] == '\n') continue;
    if (len > 0 && pattern_line[len - 1] == '\n') {
      pattern_line[len - 1] = '\0';
    }
    add_pattern(patterns, pattern_line);
  }
  fclose(pattern_file);
}

int is_regular_file(const char *filename) {
  int res = 1;
  FILE *f = fopen(filename, "r");
  if (f == NULL)
    res = 0;
  else
    fclose(f);
  return res;
}

void parse_options(int argc, char **argv, flags *opts, char **patterns) {
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "ivclnsof:e:h", 0, 0)) != -1) {
    switch (opt) {
      case 'i':
        opts->i = 1;
        break;
      case 'v':
        opts->v = 1;
        break;
      case 'c':
        opts->c = 1;
        break;
      case 'l':
        opts->l = 1;
        break;
      case 'n':
        opts->n = 1;
        break;
      case 's':
        opts->s = 1;
        break;
      case 'f':
        opts->f = 1;
        get_patterns_file(optarg, patterns);
        break;
      case 'o':
        opts->o = 1;
        break;
      case 'h':
        opts->h = 1;
        break;
      case 'e':
        opts->e = 1;
        add_pattern(patterns, optarg);
        break;
      default:
        printf("Unknown flag: %c\n", opt);
    }
  }
}

void add_pattern(char **patterns, const char *pattern) {
  int old_len = strlen(*patterns);
  int add_len = strlen(pattern);
  *patterns =
      (char *)realloc(*patterns, (old_len + add_len + 2) * sizeof(char));
  strcat(*patterns, pattern);
  strcat(*patterns, "|");
}

void output(FILE *file, const char *filename, char *patterns, flags opts) {
  char line[1024];
  int flag = 1;
  int line_number = 0;
  int match_count = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    line_number++;
    if (grep(line, patterns, filename, opts, &match_count, line_number) == -1) {
      free(patterns);
      fclose(file);
      flag = 0;
    }
  }

  if (opts.c && opts.print_filename && !opts.l && flag) {
    printf("%s:%d\n", filename, match_count);
  } else if (opts.l && match_count > 0 && flag) {
    printf("%s\n", filename);
  } else if (opts.c && !opts.l && flag) {
    printf("%d\n", match_count);
  }
}