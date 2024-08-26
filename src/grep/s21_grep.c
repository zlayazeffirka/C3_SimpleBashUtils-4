#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags opts = {0};
  char *patterns = (char *)calloc(1, 1 * sizeof(char));
  parse_options(argc, argv, &opts, &patterns);

  if (argc < 3 || optind >= argc) {
    printf("Usage: ./grep [options] template [file_name]\n");
    free(patterns);
    return 1;
  }

  int fnameid = optind;
  if (!is_regular_file(argv[optind])) {
    fnameid = optind + 1;
    add_pattern(&patterns, argv[optind]);
  }
  if (strlen(patterns) == 0) {
    printf("No pattern\n");
    free(patterns);
    return 1;
  }
  patterns[strlen(patterns) - 1] = '\0';

  if (fnameid + 1 != argc && !opts.h) {
    opts.print_filename = 1;
  }
  if (opts.o && opts.v && !opts.c && !opts.l) {
    fnameid = argc;
  }

  for (int i = fnameid; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL) {
      if (!opts.s) {
        perror(argv[i]);
      }
      continue;
    }
    output(file, argv[i], patterns, opts);
    fclose(file);
  }
  free(patterns);
  return 0;
}
