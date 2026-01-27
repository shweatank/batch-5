savedcmd_/home/dell/device/27-1/waitq_irq.mod := printf '%s\n'   waitq_irq.o | awk '!x[$$0]++ { print("/home/dell/device/27-1/"$$0) }' > /home/dell/device/27-1/waitq_irq.mod
