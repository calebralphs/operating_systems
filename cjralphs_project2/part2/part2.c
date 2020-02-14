#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <asm/current.h>
#include <asm/errno.h>
#include "part2.h"

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall2)(void);

// sys_cs3013_syscall1
asmlinkage long new_sys_cs3013_syscall2(unsigned short *target_pid, struct ancestry *response) {
    unsigned short pid;
    if(copy_from_user(&pid, target_pid, sizeof(unsigned short))) return EFAULT;
    int i = 0;
    struct task_struct* target_task = pid_task(find_vpid(pid), PIDTYPE_PID);
    int current_pid = target_task->pid;
    struct list_head* child_head = &(target_task->children);
    struct list_head* sibling_head = &(target_task->sibling);

    struct ancestry response_kernel_space;
    struct ancestry response_from_user;
    // ensure we have array pointers inside the struct
    if (copy_from_user(&response_from_user, response, sizeof(struct ancestry))) {
        return EFAULT;
    }
    // build out response_kernel_space;

    printk(KERN_INFO "Current: %d\n", current_pid);
    while (current_pid > 1 && i < 10) {
        target_task = target_task->parent;
        current_pid = target_task->pid;
	    response_kernel_space.ancestors[i] = current_pid;
        i += 1;
    }
    for (i = 0; i < 10; i++) {
        printk(KERN_INFO "Ancestor %d: %d\n", i, response_kernel_space.ancestors[i]);
    }
    
    i = 0;
    // Children
    struct task_struct *child_task; 
    struct list_head *cursor;
    list_for_each(cursor, &(target_task->children)) { 
        if (i >= 100) break;
        child_task = list_entry(cursor, struct task_struct, sibling); 
        response_kernel_space.children[i] = child_task->pid;
        i += 1;
    }
    for (i = 0; i < 100; i++) {
        printk(KERN_INFO "Child %d: %d\n", i, response_kernel_space.children[i]);
    }

    i = 0;
    // Sibling
    struct task_struct *sibling_task; 
    list_for_each(cursor, &(target_task->sibling)) { 
        if (i >= 100) break;
        sibling_task = list_entry(cursor, struct task_struct, sibling); 
        response_kernel_space.siblings[i] = sibling_task->pid;
        i += 1;
    }
    for (i = 0; i < 100; i++) {
        printk(KERN_INFO "Sibling %d: %d\n", i, response_kernel_space.siblings[i]);
    }

    if (copy_to_user(response, &response_kernel_space, sizeof(struct ancestry))) {
        return EFAULT;
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
