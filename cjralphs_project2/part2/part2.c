#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <asm/current.h>
#include "part2.h"

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall2)(void);

// sys_cs3013_syscall1
asmlinkage long new_sys_cs3013_syscall2(unsigned short *target_pid) {
    struct task_struct* current_task = current;
    int current_pid = current_task->pid;
    printk(KERN_INFO "CURRENT TASK STRUCT PID: %d\n", current_pid);

    int i = 0;
    while (current_pid > 1) {
        current_task = current_task->parent;
        current_pid = current_task->pid;
        printk(KERN_INFO "PARENT %d TASK STRUCT PID: %d\n", i, current_pid);
        i += 1;
    }

    return 0;
}

static unsigned long **find_sys_call_table(void) {
    unsigned long int offset = PAGE_OFFSET;
    unsigned long **sct;

    while (offset < ULLONG_MAX) {
        sct = (unsigned long **)offset;

        if (sct[__NR_close] == (unsigned long *)sys_close) {
            printk(KERN_INFO "Interceptor: Found syscall table at address: 0x%02lX", (unsigned long)sct);
            return sct;
        }

        offset += sizeof(void *);
    }

    return NULL;
}

static void disable_page_protection(void) {
    /*
    Control Register 0 (cr0) governs how the CPU operates.

    Bit #16, if set, prevents the CPU from writing to memory marked as
    read only. Well, our system call table meets that description.
    But, we can simply turn off this bit in cr0 to allow us to make
    changes. We read in the current value of the register (32 or 64
    bits wide), and AND that with a value where all bits are 0 except
    the 16th bit (using a negation operation), causing the write_cr0
    value to have the 16th bit cleared (with all other bits staying
    the same. We will thus be able to write to the protected memory.

    It's good to be the kernel!
    */
    write_cr0(read_cr0() & (~0x10000));
}

static void enable_page_protection(void) {
    /*
    See the above description for cr0. Here, we use an OR to set the 
    16th bit to re-enable write protection on the CPU.
    */
    write_cr0(read_cr0() | 0x10000);
}

static int __init interceptor_start(void) {
    /* Find the system call table */
    if (!(sys_call_table = find_sys_call_table())) {
        /* Well, that didn't work. 
        Cancel the module loading step. */
        return -1;
    }

    /* Store a copy of all the existing functions */
    ref_sys_cs3013_syscall2 = (void *)sys_call_table[__NR_cs3013_syscall2];


    /* Replace the existing system calls */
    disable_page_protection();
    sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)new_sys_cs3013_syscall2;
    enable_page_protection();

    /* And indicate the load was successful */
    printk(KERN_INFO "Loaded interceptor!");

    return 0;
}

static void __exit interceptor_end(void) {
    /* If we don't know what the syscall table is, don't bother. */
    if (!sys_call_table) {
        return;
    }

    /* Revert all system calls to what they were before we began. */
    disable_page_protection();
    sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)ref_sys_cs3013_syscall2;
    enable_page_protection();

    printk(KERN_INFO "Unloaded interceptor!");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);