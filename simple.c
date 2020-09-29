/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday
{
	int day;
	int month;
	int year;
	struct list_head list;
};

	static LIST_HEAD(birthday_list);

void add_birthday(int day, int month, int year)
{
	struct birthday *person;

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = day;
	person->month = month;
	person->year = year;

	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
	struct birthday *ptr;

	add_birthday(2, 8, 1995);
	add_birthday(1, 5, 1998);
	add_birthday(7, 3, 1992);
	add_birthday(25, 3, 1991);
	add_birthday(15, 11, 1926);

	list_for_each_entry(ptr, &birthday_list, list)
	{
		/* on each iteration ptr points */
		printk(KERN_INFO "m/d/y:  %d %d %d\n", ptr->month, ptr->day, ptr->year);
		/* to the next birthday struct  */
	}

        printk(KERN_INFO "Loading Module\n");

        return 0;
}



/* This function is called when the module is removed. */
void simple_exit(void)
{
	struct birthday *ptr, *next;

	list_for_each_entry_safe(ptr,next,&birthday_list,list)
	{
		/* on each iteration ptr points */
		/* to the next birthday struct  */
		list_del(&ptr->list);
		kfree(ptr);
	}
	printk(KERN_INFO "Print after kfree:\n");
	list_for_each_entry(ptr, &birthday_list, list)
	{
		printk(KERN_INFO "m/d/y: %d %d %d\n", ptr->month, ptr->day, ptr->year);
	}

	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

