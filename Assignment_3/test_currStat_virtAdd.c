#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

#define __NR_currStat_virtAdd 341

int main(int argc, char *argv[])
{
        int pid;
        unsigned long virtAdd;

        printf("please enter your desired virtual address. > ");
        scanf("%lu", &virtAdd);
        printf("please enter your desired PID. > ");
        scanf("%d", &pid);
        syscall(__NR_currStat_virtAdd, virtAdd, pid);

        return 0;
}