savedcmd_/home/dell/device/22-1/kmalloc_use.mod := printf '%s\n'   kmalloc_use.o | awk '!x[$$0]++ { print("/home/dell/device/22-1/"$$0) }' > /home/dell/device/22-1/kmalloc_use.mod
