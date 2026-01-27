savedcmd_/home/dell/device/27-1/workq.mod := printf '%s\n'   workq.o | awk '!x[$$0]++ { print("/home/dell/device/27-1/"$$0) }' > /home/dell/device/27-1/workq.mod
