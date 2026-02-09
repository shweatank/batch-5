#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

struct Student {
    int id;
    int marks; 
};

static int __init kmalloc_example_init(void) {
    struct Student *s;

    printk(KERN_INFO "kmalloc module loaded\n");

    s = kmalloc(sizeof(struct Student), GFP_KERNEL);
    if (!s) {
        printk(KERN_ALERT "Memory allocation failed\n");
        return -ENOMEM;
    }

    s->id = 101;
    s->marks = 8850; 

    printk(KERN_INFO "ID: %d\n", s->id);
    printk(KERN_INFO "Marks: %d.%02d\n", s->marks / 100, s->marks % 100);

    kfree(s);
    return 0;
}

static void __exit kmalloc_example_exit(void) {
    printk(KERN_INFO "kmalloc module unloaded\n");
}

module_init(kmalloc_example_init);
module_exit(kmalloc_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("kmalloc example with struct");

