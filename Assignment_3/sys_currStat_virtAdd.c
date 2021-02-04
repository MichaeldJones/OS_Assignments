#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_type.h>
#include <linux/highmem.h>

asmlinkage int sys_currStat_virtAdd(unsigned long virtAdd, int pid)
{

	struct task_struct *task;

	pgd_t *pgd;
	pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;
    spinlock_t *lock;

	pgd = pgd_offset(task->mm, virtAdd);
	pud = pud_offset(pgd, virtAdd);
	pmd = pmd_offset(pud, virtAdd);
	ptep = pte_offset(task->mm, pmd, virtAdd, &lock);
	pte = *ptep;

	pte_unmap_unlock(ptep , lock); 

	if(pte_present(pte))
		printk("this PID is stored in memory");
	else
		printk("this PID is stored on disk");

	return 0;
}


