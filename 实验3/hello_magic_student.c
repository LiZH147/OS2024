/* hello_magic_student form Krenel! */
#include <linux/module.h>
MODULE_LICENSE("GPL");
static int id = 0;
module_param(id, int, 0644);
MODULE_PARM_DESC(id, "int param\n");
static char* name = "Kernel";
module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "char* param\n");
static int age = 21;
module_param(age, int, 0644);
MODULE_PARM_DESC(age, "int param\n");
void hello_student(int id, char* name, int age){
    printk(KERN_ALERT "My name is %s, student id is %d, I am %d years old.\n", name, id, age);
}
void hello_magic_student(int id, int age){
    printk(KERN_ALERT "My magic number is %d.\n", id + age);
}

int __init hello_init(void)
{
printk(KERN_ALERT "Init module.\n");
hello_student(id, name, age);
hello_magic_student(id, age);
return 0;
}
void __exit hello_exit(void)
{
printk(KERN_ALERT "Exit module.\n");
}
module_init(hello_init);
module_exit(hello_exit);
