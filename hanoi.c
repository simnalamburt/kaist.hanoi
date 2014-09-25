#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "hanoi.h"

typedef struct job {
  int count;  // 옮길 판들의 갯수 (자연수)
  int src;    // 옮길 판들의 출발지 (0, 1, 2)
  int dst;    // 옮길 판들의 목적지 (0, 1, 2)
  int number; // 옮길 판들 가운데 제일 큰 판의 번호 (0, 1, ... count)
} job;

static job _stack[128];
static job* _stack_top = _stack;

static bool empty() {
  return _stack_top == _stack;
}

static void push(int count, int src, int dst, int number) {
  (*_stack_top).count   = count;
  (*_stack_top).src     = src;
  (*_stack_top).dst     = dst;
  (*_stack_top).number  = number;
  ++_stack_top;
}

static job pop() {
  return *--_stack_top;
}

static void dump() {
  printf("_stack = [");
  for (struct job* it=_stack; it<_stack_top; ++it) {
    printf("(%d, %d, %d, %d), ", it->count, it->src, it->dst, it->number);
  }
  puts("]");
}

static void solve(int count) {
  int i = 1, third;

  printf("N=%d, Time stamp: %d\n", count, 0);
  push(count, 0, 1, count);

  do {
    job job = pop();

    if (job.count == 1) {
      printf("%d: Disk %d from %c to %c\n",
        i++, job.number, job.src + 'a', job.dst + 'a');
    } else {
      third = 3 - job.src - job.dst;

      push(job.count-1, third, job.dst, job.count-1);   // Step 3
      push(1, job.src, job.dst, job.count);         // Step 2
      push(job.count-1, job.src, third, job.count-1);   // Step 1
    }

  } while (!empty());

  putchar('\n');
}

void hanoi(char* filename) {
  FILE* f = fopen(filename, "r");

  char buffer[128];
  fgets(buffer, 128, f);

  int count;
  while (fscanf(f, "%d", &count) != EOF) {
    solve(count);
  }

  fclose(f);
}
