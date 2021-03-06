#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "hanoi.h"


void wrong_args(const char*);
void regex_init_failed(void);
void regex_match_failed(regex_t*, int);


bool match(const char* target, const char* exp) {
  regex_t regex;
  int ret;

  ret = regcomp(&regex, exp, REG_EXTENDED);
  if (ret) { regex_init_failed(); }

  bool result;
  switch(ret = regexec(&regex, target, 0, 0, 0)) {
    case 0:
      result = true;
      break;
    case REG_NOMATCH:
      result = false;
      break;
    default:
      regex_match_failed(&regex, ret);
  }

  regfree(&regex);
  return result;
}

int main(int argc, char** argv) {
  if (argc != 2) { wrong_args(argv[0]); }
  const char* input = argv[1];

  int start, count;
  if (match(input, "^[0-9]+$")) {
    start = strtol(input, 0, 10);
    count = 1;
  } else if (match(input, "^[0-9]+-[0-9]+$")) {
    char* delim = strchr(input, '-');

    start = strtol(input, &delim, 10);
    count = strtol(delim+1, 0, 10) - start + 1;
  } else {
    wrong_args(argv[0]);
  }

  hanoi(start, count);
  return 0;
}


// Error cases
void wrong_args(const char* arg0) {
  fprintf(stderr, "Arguments should be like:\n");
  fprintf(stderr, "\033[33m");
  fprintf(stderr, "  %s 30\n", arg0);
  fprintf(stderr, "  %s 0-20\n", arg0);
  fprintf(stderr, "  %s 25-27\n", arg0);
  fprintf(stderr, "\033[0m");
  exit(1);
}

void regex_init_failed(void) {
  fprintf(stderr, "Could not compile regex\n");
  exit(2);
}

void regex_match_failed(regex_t* pregex, int error_code) {
  char msgbuf[128];

  regerror(error_code, pregex, msgbuf, sizeof msgbuf);
  fprintf(stderr, "Regex match failed: %s\n", msgbuf);
  exit(3);
}
