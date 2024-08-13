#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    parser(argc, argv);
    execute(argc, argv);
  }
  return 0;
}

void get_error() {
  if (!options.s) {
    fprintf(stderr, "%s\n", "Error.");
  }
}

void grep_f_flag(char *file_name) {
  FILE *filed;
  char *line;
  char result[1000] = "";
  filed = fopen(file_name, "r");
  if (filed) {
    while ((line = fgets(result, 1000, filed))) {
      if (line_is_empty(line)) pattern.empty_true = 1;
      line[strcspn(result, "\n")] = 0;
      strcpy(pattern.name[pattern.count++], line);
    }
  } else {
    get_error();
  }
  fclose(filed);
}

void copy_pattern(char *str) {
  str[strcspn(str, "\n")] = 0;
  strcpy(pattern.name[pattern.count++], str);
}

void parser(int argc, char **argv) {
  int opt;
  while ((opt = getopt_long(argc, argv, "f:e:ivclnhso", NULL, NULL)) != -1) {
    if (opt == 'f') {
      options.f = 1;
      grep_f_flag(optarg);
    }
    if (opt == 'e') {
      options.e = 1;
      copy_pattern(optarg);
    }
    if (opt == 'i') {
      options.i = 1;
    }
    if (opt == 'v') {
      options.v = 1;
    }
    if (opt == 'c') {
      options.c = 1;
    }
    if (opt == 'l') {
      options.l = 1;
    }
    if (opt == 'n') {
      options.n = 1;
    }
    if (opt == 'h') {
      options.h = 1;
    }
    if (opt == 's') {
      options.s = 1;
    }
    if (opt == 'o') {
      options.o = 1;
    }
    if (opt == '?') {
      get_error();
    }
  }

  if (options.v || options.c || options.l) {
    options.o = 0;
  }
}

int line_is_empty(char *line) {
  int res = 0;
  if (strlen(line) == 1 && line[0] == '\n') res = 1;
  return res;
}

void change_line(char *line) {
  if (strlen(line) == 1 && line[0] == '\n') line[0] = '.';
  if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
}
void print_file_name() {
  if (file.count_all > 1 && !options.h) {
    printf("%s:", file.name[file.count - 1]);
  }
}

void flag_c() {
  if (!options.h && file.count_all > 1)
    printf("%s:", file.name[file.count - 1]);
  if (!options.l)
    printf("%d\n", file.matched_lines);
  else
    printf("%d\n", file.matched_lines > 0 ? 1 : 0);
}

void flag_l() {
  if (file.matched_lines > 0) printf("%s\n", file.name[file.count - 1]);
}

void get_line_file() {
  FILE *filed;
  filed = fopen(file.name[file.count - 1], "r");
  file.line_number = 0;
  file.matched_lines = 0;
  char result[1000] = "";
  if (filed) {
    char *lines;
    while ((lines = fgets(result, 1000, filed))) {
      file.file_was_print = 1;
      strcpy(file.line, lines);
      change_line(file.line);
      file.line_number++;
      if (pattern.empty_true) {
        if (options.l || options.c || options.o || !(options.o && options.v)) {
          print_all();
        }
      } else {
        seek_in_file();
      }
    }
    if (options.c) flag_c();
    if (options.l) flag_l();
    fclose(filed);
  } else {
    get_error();
  }
}

void seek_in_file() {
  int i = 0;
  while (i < pattern.count) {
    pattern.current_pattern = i;
    i++;
    execute_pattern();
  }
  if (options.v && file.file_was_print == 1) {
    print_match();
  }
}

void print_match() {
  if (!options.c && !options.l && file.file_was_print) {
    if (strlen(file.line) == 1 && file.line[0] == '.') file.line[0] = '\0';
    if (file.count_all > 1) print_file_name();
    if (options.n) printf("%d:", file.line_number);
    printf("%s\n", file.line);
  }
  if (file.file_was_print) file.matched_lines++;
}

void flag_o() {
  regmatch_t pmatch[1] = {0};
  regex_t preg = {0};
  char *bine;
  bine = file.line;
  if (regcomp(&preg, pattern.name[pattern.current_pattern],
              options.i ? REG_EXTENDED | REG_NEWLINE | REG_ICASE
                        : REG_EXTENDED | REG_NEWLINE) == 0) {
    while (regexec(&preg, bine, 1, pmatch, 0) == 0) {
      char result[1000] = "";
      if (!file.file_name_print) {
        print_file_name();
        if (options.n) printf("%d:", file.line_number);
      }
      int len = (int)(pmatch[0].rm_eo - pmatch[0].rm_so);
      memcpy(result, bine + pmatch[0].rm_so, len);
      printf("%s\n", result);
      bine += pmatch[0].rm_eo;
    }
  } else {
    get_error();
  }
  regfree(&preg);
}

void execute_pattern() {
  regex_t preg = {0};
  if (options.o) {
    flag_o();
  } else {
    if (regcomp(&preg, pattern.name[pattern.current_pattern],
                options.i ? REG_EXTENDED | REG_ICASE : REG_EXTENDED) == 0) {
      if (regexec(&preg, file.line, 0, NULL, 0) == 0) {
        if (!options.v) {
          print_match();
        }
        file.file_was_print = 0;
      }
    } else {
      get_error();
    }
    regfree(&preg);
  }
}

void print_all() {
  if (!options.c && !options.l) {
    if (strlen(file.line) == 1 && file.line[0] == '.') file.line[0] = '\0';
    print_file_name();
    file.matched_lines++;
    if (options.n) printf("%d: ", file.line_number);
    printf("%s\n", file.line);
  }
}

void execute(int argc, char **argv) {
  if (!(options.e) && !(options.f)) {
    copy_pattern(argv[optind++]);
  }
  file.count_all = argc - optind;
  while (optind < argc) {
    strcpy(file.name[file.count++], argv[optind++]);
    get_line_file();
  }
}
