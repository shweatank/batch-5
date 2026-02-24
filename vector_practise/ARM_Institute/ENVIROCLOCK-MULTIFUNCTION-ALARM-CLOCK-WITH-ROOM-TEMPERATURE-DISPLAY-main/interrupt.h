#include "types.h"

void interrupt_configuration(void);
void display_menu(void);
void eint1_isr(void)__irq;
void menu_display(s32 *,s32 *);
