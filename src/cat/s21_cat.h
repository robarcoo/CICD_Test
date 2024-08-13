#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;  // нумерует все непустые строки
  int e;  // пишет в конце $
  int s;  // сжимает пустые строки
  int t;  // заменяет табы на ^I
  int n;  // нумерует все строки
  int v;  // печатает все непечатаемые символы
} opt_t;

int parser(int argc, char **argv, opt_t *options);
void reader(int argc, char **argv, opt_t *options);
