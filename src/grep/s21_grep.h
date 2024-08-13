#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

struct opt_t {
  int e;  //  done
  int i;  //  done
  int v;  //  done
  int c;  //  done
  int l;  //  done
  int n;  //  done
  int h;  //  done
  int s;  //  done
  int f;  //  done
  int o;  //  done
};

struct patterns {
  int count;
  char name[1000][1000];
  int current_pattern;
  int empty_true;
};

struct files {
  int count;
  char name[1000][1000];
  int l_true;
  char line[1000];
  int line_number;
  int matched_lines;
  int file_was_print;
  int file_name_print;
  int count_all;
};

struct opt_t options = {0};
struct patterns pattern = {0};
struct files file = {0};

void parser();
void execute();
void copy_pattern(char* str);
void copy_file_pattern(const char* str);
void print_all();
void execute_pattern();
void print_match();
void seek_in_file();
int line_is_empty(char* line);

#endif  // SRC_GREP_S21_GREP_H_
