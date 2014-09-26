#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "hanoi.h"


static void nop() {
  static int _i = 0;
  ++_i;
}


// 재귀
void solve_rec(int count, int src, int dst, int number) {
  //  count : 옮길 판들의 갯수                   (자연수)
  //    src : 옮길 판들의 출발지                 (0, 1, 2)
  //    dst : 옮길 판들의 목적지                 (0, 1, 2)
  // number : 옮길 판들 가운데 제일 큰 판의 번호 (0, 1, ... count)

  if (count <= 0) {
    return;
  } else if (count == 1) {
    nop();
  } else {
    int tmp = 3 - src - dst;

    solve_rec(count - 1, src, tmp, number - 1);
    solve_rec(1,         src, dst, number);
    solve_rec(count - 1, tmp, dst, number - 1);
  }
}

static void solve1(int N) {
  solve_rec(N, 0, 1, N);
}


// 반복
typedef struct job {
  int count;  // 옮길 판들의 갯수                   (자연수)
  int src;    // 옮길 판들의 출발지                 (0, 1, 2)
  int dst;    // 옮길 판들의 목적지                 (0, 1, 2)
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

static void solve2(int N) {
  push(N, 0, 1, N);
  do {
    job job = pop();

    if (job.count <= 0) {
      continue;
    } else if (job.count == 1) {
      nop();
    } else {
      int tmp = 3 - job.src - job.dst;

      push(job.count - 1, tmp,     job.dst, job.number - 1);  // Step 3
      push(1,             job.src, job.dst, job.number);      // Step 2
      push(job.count - 1, job.src, tmp,     job.number - 1);  // Step 1
    }
  } while (!empty());
}


// 테스트
static double utime() {
  struct rusage ru;
  getrusage(RUSAGE_SELF, &ru);
  return ru.ru_utime.tv_sec + ru.ru_utime.tv_usec / 1000000.0;
}

static double _begin;
static void start() { _begin = utime(); }
static double done() { return utime() - _begin; }

void hanoi(int begin, int count) {
  const char* g = "\033[32m";
  const char* y = "\033[33m";
  const char* r = "\033[0m";

  for (int i = begin; i < begin + count; ++i) {
    start();
    solve1(i);
    double t1 = done();

    start();
    solve2(i);
    double t2 = done();

    printf("N = %s%2d%s   recursive = %s%lf%s sec   iterative = %s%lf%s\n",
      g, i, r, y, t1, r, y, t2, r);
  }
}
