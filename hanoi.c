#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include "hanoi.h"


// Helper functions
static int _i;
static double _begin;

static double utime() {
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

static void start(int count) {
  printf("N = \033[32m%2d\033[0m", count);
  _i = 1;
  _begin = utime();
}

static void result(int src, int dst, int number) {
  ++_i; // No opti
}

static void done() {
  printf("   time = \033[33m%lf\033[0m sec\n", utime() - _begin);
}


// 재귀
void solve_rec(int count, int src, int dst, int number) {
  //  count : 옮길 판들의 갯수                   (자연수)
  //    src : 옮길 판들의 출발지                 (0, 1, 2)
  //    dst : 옮길 판들의 목적지                 (0, 1, 2)
  // number : 옮길 판들 가운데 제일 큰 판의 번호 (0, 1, ... count)

  if (count == 1) {
    result(src, dst, number);
  } else {
    int tmp = 3 - src - dst;

    solve_rec(count - 1, src, tmp, number - 1);
    solve_rec(1,         src, dst, number);
    solve_rec(count - 1, tmp, dst, number - 1);
  }
}

static void solve1(int N) {
  start(N);
  solve_rec(N, 0, 1, N);
  done();
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
  start(N);

  push(N, 0, 1, N);
  do {
    job job = pop();

    if (job.count == 1) {
      result(job.src, job.dst, job.number);
    } else {
      int tmp = 3 - job.src - job.dst;

      push(job.count - 1, tmp,     job.dst, job.number - 1);  // Step 3
      push(1,             job.src, job.dst, job.number);      // Step 2
      push(job.count - 1, job.src, tmp,     job.number - 1);  // Step 1
    }
  } while (!empty());

  done();
}


// 테스트
void test(char* filename, void (*testee)(int)) {
  int c;
  FILE* f = fopen(filename, "r");

  // 첫번째줄 생략
  do { c = fgetc(f); } while(c != '\n' && c != EOF);

  // 정수 입력받아 테스트 반복
  while (fscanf(f, "%d", &c) != EOF) {
    testee(c);
  }

  fclose(f);
}

void hanoi(char* filename) {
  puts(" --- 재귀 ---");
  test(filename, solve1);
  puts("");
  puts(" --- 반복 ---");
  test(filename, solve2);
}
