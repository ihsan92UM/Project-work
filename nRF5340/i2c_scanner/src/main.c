#include <zephyr/kernel.h>
#include<zephyr/sys/printk.h>
int main(void)
{
#ifdef CONFIG_PRINTK
	printk("Hello World from minimal!\n");
#endif

	return 0;
}