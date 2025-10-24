// user/strace.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "usage: strace <prog> [args...]\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "strace: fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    // child: exec 目標程式（argv[1]..）
    exec(argv[1], &argv[1]);
    fprintf(2, "strace: exec %s failed\n", argv[1]);
    exit(1);
  }

  // parent: 對 child 開啟追蹤並等待
  if (trace(pid) < 0) {
    fprintf(2, "strace: trace(%d) failed\n", pid);
    // 仍然等 child 回收
  }
  wait(0);
  exit(0);
}
