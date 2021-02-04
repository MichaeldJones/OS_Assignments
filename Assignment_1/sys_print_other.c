#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>

asmlinkage int sys_print_other(int target)
{
  struct task_struct *task;
  
  for(task = current; task != &init_task; task = task->parent)
  {
    if(task->pid == target)
    {
      printk("%s: %d\n\n", task->comm, task->pid);
      return 0;
    }
  }

  printk("PID not found\n\n");
  return 0;
}
