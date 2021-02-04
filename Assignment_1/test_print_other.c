#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define __NR_print_other   339

int main(int argc, char *argv[])
{
  int PID;

  printf("Enter PID > ");
  scanf("%d", &PID);
  syscall(__NR_print_other, PID);
  return 0;
}

