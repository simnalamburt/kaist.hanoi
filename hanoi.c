#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "hanoi.h"


// 시간 측정
static double time_in_float() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (tv.tv_sec) * 1000.0 + (tv.tv_usec) / 1000.0;
}


// 재귀버전: solve1(n)
static int i;
static void solve_rec(int count, int src, int dst) {
  int tmp;

  if (count == 1) {
    printf("%d: Disk %d from %c to %c\n", i++, count, src + 'a', dst + 'a');
  } else {
    tmp = 3 - src - dst;
    solve_rec(count - 1, src, tmp);
    printf("%d: Disk %d from %c to %c\n", i++, count, src + 'a', dst + 'a');
    solve_rec(count - 1, tmp, dst);
  }
}

static void solve1(int count) {
  printf("N=%d, Time stamp: %lf\n", count, time_in_float());

  i = 1;
  solve_rec(count, 0, 1);

  putchar('\n');
}


// 반복문버전: solve2(n)
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

static void solve2(int count) {
  int i = 1, third;

  printf("N=%d, Time stamp: %lf\n", count, time_in_float());
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
    // solve1(n) 혹은 solve2(n) 호출하면 됨
    solve1(count);
  }

  fclose(f);
}
