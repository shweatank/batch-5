savedcmd_/home/dell/device/23-1/irq_module.mod := printf '%s\n'   irq_module.o | awk '!x[$$0]++ { print("/home/dell/device/23-1/"$$0) }' > /home/dell/device/23-1/irq_module.mod
