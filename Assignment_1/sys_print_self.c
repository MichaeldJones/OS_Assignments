#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>

asmlinkage int sys_print_self(void)
{
  struct task_struct *task;
  
  //printk("Starting...\n\n");
  
  //printk("current: %s %d\n", current->comm, current->pid);
  //printk("init: %s %d\n", init_task.comm, init_task.pid);
  
  for(task = current; task != &init_task; task = task->parent)
  {
    printk("%s: %d\n", task->comm, task->pid);
  }
  return 0;
}
