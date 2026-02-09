#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/input.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swathi");
MODULE_DESCRIPTION("Keyboard operation driver (a=add, s=sub, m=mul, d=div)");

static int num1 = 0, num2 = 0;
static char op = 0;
static int second = 0;

static void reset_calc(void)
{
    num1 = 0;
    num2 = 0;
    op = 0;
    second = 0;
}

static int keyboard_handler(struct notifier_block *nblock,
                            unsigned long code, void *param)
{
    struct keyboard_notifier_param *kp = param;

    if (code != KBD_KEYCODE || !kp->down)
        return NOTIFY_OK;

    /* digits */
    if (kp->value >= KEY_1 && kp->value <= KEY_9) {
        int digit = kp->value - KEY_1 + 1;
        if (!second) {
            num1 = num1 * 10 + digit;
            pr_info("Num1 = %d\n", num1);
        } else {
            num2 = num2 * 10 + digit;
            pr_info("Num2 = %d\n", num2);
        }
    }
    else if (kp->value == KEY_0) {
        if (!second) {
            num1 = num1 * 10;
            pr_info("Num1 = %d\n", num1);
        } else {
            num2 = num2 * 10;
            pr_info("Num2 = %d\n", num2);
        }
    }

    /* operations */
    else if (kp->value == KEY_A) {
        op = '+';
        second = 1;
        pr_info("Operator = +\n");
    }
    else if (kp->value == KEY_S) {
        op = '-';
        second = 1;
        pr_info("Operator = -\n");
    }
    else if (kp->value == KEY_M) {
        op = '*';
        second = 1;
        pr_info("Operator = *\n");
    }
    else if (kp->value == KEY_D) {
        op = '/';
        second = 1;
        pr_info("Operator = /\n");
    }

    /* Enter = calculate */
    else if (kp->value == KEY_ENTER) {
        if (op == 0) {
            pr_info("Invalid operation\n");
        } else {
            int res = 0;
            switch (op) {
                case '+': res = num1 + num2; break;
                case '-': res = num1 - num2; break;
                case '*': res = num1 * num2; break;
                case '/':
                    if (num2 == 0)
                        pr_info("Division by zero!\n");
                    else
                        res = num1 / num2;
                    break;
            }
            pr_info("Result = %d\n", res);
        }
        reset_calc();
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = keyboard_handler
};

static int __init keyboard_irq_init(void)
{
    pr_info("Loading Keyboard Operation Driver...\n");
    register_keyboard_notifier(&nb);
    pr_info("Driver loaded successfully.\n");
    return 0;
}

static void __exit keyboard_irq_exit(void)
{
    unregister_keyboard_notifier(&nb);
    pr_info("Driver removed.\n");
}

module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

