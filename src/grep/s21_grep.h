#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int i;
  int v;
  int c;
  int l;
  int n;
  int s;
  int o;
  int h;
  int e;
  int f;
  int print_filename;
} flags;

int find_matches(char *patterns, const char *line, flags opts,
                 const char *filename, int line_number);
int grep(const char *line, char *patterns, const char *filename, flags opts,
         int *match_count, int line_number);
void get_patterns_file(const char *filename, char **patterns);
void parse_options(int argc, char **argv, flags *opts, char **patterns);
void output(FILE *file, const char *filename, char *patterns, flags opts);
void add_pattern(char **patterns, const char *pattern);
int is_regular_file(const char *filename);

#endif