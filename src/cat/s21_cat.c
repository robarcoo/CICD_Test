/*
 1. main
 2. parser
 3. reader
 */
#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    opt_t options = {0};
    int err_true = parser(argc, argv, &options);
    if (!err_true) {
      reader(argc, argv, &options);
    }
  }
  return 0;
}

int parser(int argc, char **argv, opt_t *options) {
  int opt;
  const struct option long_option[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};
  int option_index;
  int err_true = 0;
  while ((opt = getopt_long(argc, argv, "+bestnvTE", long_option,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
      case 'E':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
      case 'T':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [options] [file ...]\n");
        err_true = 1;
        break;
    }
  }
  return err_true;
}
void reader(int argc, char **argv, opt_t *options) {
  for (int i = optind; i < argc; i++) {
    FILE *f;
    f = fopen(argv[i], "rt");
    int cur = getc(f);  //  нынешний символ
    int prev = -1;      //  предыдущий символ
    int count = 0;      //  считаем строки
    if (f && (cur != -1)) {
      while (cur != EOF) {
        if (options->s) {
          if (cur == '\n' && prev == '\n') {
            while (cur == '\n') {
              prev = cur;
              cur = fgetc(f);
            }
            if (!options->b && options->n && options->e) {
              count++;
              printf("%6d\t$\n", count);
            } else if (!options->b && options->n) {
              count++;
              printf("%6d\t\n", count);
            } else if (options->e) {
              printf("$\n");
            } else {
              printf("\n");
            }
          }
        }
        if (!options->b && options->n) {
          if (prev == '\n' || count == 0) {
            count++;
            printf("%6d\t", count);
          }
        }
        if (options->b) {
          if ((prev == '\n' || count == 0) && cur != '\n') {
            count++;
            printf("%6d\t", count);
          }
        }
        if (options->t) {
          while (cur == '\t') {
            printf("^I");
            prev = cur;
            cur = fgetc(f);
          }
        }
        if (options->v) {
          if ((cur >= 0 && cur < 9) || (cur > 10 && cur < 32)) {
            cur += 64;
            printf("^");
          } else if (cur == 127) {
            cur -= 64;
            printf("^");
          }
        }
        if (options->e) {
          if (cur == '\n') {
            printf("$");
          }
        }
        printf("%c", cur);
        prev = cur;
        cur = fgetc(f);
      }
      fclose(f);
    }
  }
}
