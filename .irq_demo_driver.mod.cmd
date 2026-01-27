savedcmd_/home/dell/practice/irq_demo_driver.mod := printf '%s\n'   irq_demo_driver.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/irq_demo_driver.mod
