/**
 * RodriguezRodriguezLKM.c
 *
 * Get an integer argument from the loader like this: sudo insmod simple.ko inp_pid=1234
 * Prints the following:
 * - PROCESS NAME 
 * - PID
 * - STATE
 * - PRIORITY
 * - STATIC-PRIORITY
 * - NORMAL-PRIORITY
 * All of them must be in tabular format
 * PROCESS NAME \t\t PID \t\t STATE \t\t PRIO \t\t ST_PRIO \t\t NORM_PRIO
 * 
 * then show CHILD processes (direct children only)
 * then show PARENT Process
 *
 * EXAMPLE:
 * PROCESS NAME      PID      STATE         PRIO      ST_PRIO        NORM_PRIO
 * kworker           1489        1           0        0               0
 * CHILD  
 * kworker           1490        1           0        0               0
 * PARENT
 * kworker           1490        1           0        0               0
 * 
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 *
 * Completion time: 4h
 *
 * @author Claudio Rodriguez Rodriguez, Silberschatz, Galvin, and Gagne from Operating System Concepts 
 * @version 01/30/2022
 */
// MODULE_LICENSE
// MODULE_DESCRIPTION
// MODULE_AUTHOR
// module_init
// module_exit
// module_param 
// MODULE_PARM_DESC
#include <linux/module.h>
// task_struct
#include <linux/sched.h>
// for_each_process
#include <linux/sched/signal.h>
// already included on module.h but added for tracking purposes
#include <linux/list.h> 
// already included on module.h but added for tracking purposes
#include <linux/moduleparam.h>
// already included on module.h but added for tracking purposes
#include <linux/kernel.h>

#define MODULE_NAME "RodriguezRodriguezLKM"

static int inp_pid = 0;

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(MODULE_NAME" Module for SER334");
MODULE_AUTHOR("claudiordgz <cbrodri1@asu.edu>");
module_param(inp_pid, int, 0);
MODULE_PARM_DESC(inp_pid, " Gets processes with PID greater than this integer.");

void print_header(void);
void print_row(struct task_struct *task);

int init_rodriguez_rodriguez_lkm_module(void)
{
    struct task_struct *task;
    struct list_head *p;
    struct task_struct *t;

    printk(KERN_INFO "Loading Module: %s\n", MODULE_NAME);
    printk(KERN_INFO "inp_pid = %i\n", inp_pid);
    for_each_process(task) {
        if (task->pid >= inp_pid) {
            print_header();
            print_row(task);
            
            list_for_each(p, &task->children) {
                t = list_entry(p, struct task_struct, sibling);
                printk(KERN_INFO "CHILD\n");
                print_row(t);
            }

            printk(KERN_INFO "PARENT\n");
            print_row(task->parent);
            printk(KERN_INFO " \n");
        }
    }
    return 0;
}

void exit_rodriguez_rodriguez_lkm_module(void) {
	printk(KERN_INFO "Removing Module: %s\n", MODULE_NAME);
}

void print_header(void) {
    printk(KERN_INFO "%-17s %-9s %5s %-4s %-8s %-5s\n", "PROCCESS","PID","STATE","PRIO","ST_PRIO","NORM_PRIO");
}

void print_row(struct task_struct *task) {
    printk(KERN_INFO "%-17s %-9i %5li %-4i %-8i %-5i\n", task->comm, task->pid, task->state, task->prio, task->static_prio, task->normal_prio);
}


module_init( init_rodriguez_rodriguez_lkm_module );
module_exit( exit_rodriguez_rodriguez_lkm_module );
