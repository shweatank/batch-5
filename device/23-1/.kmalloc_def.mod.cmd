savedcmd_/home/dell/device/23-1/kmalloc_def.mod := printf '%s\n'   kmalloc_def.o | awk '!x[$$0]++ { print("/home/dell/device/23-1/"$$0) }' > /home/dell/device/23-1/kmalloc_def.mod
