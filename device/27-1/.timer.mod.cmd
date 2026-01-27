savedcmd_/home/dell/device/27-1/timer.mod := printf '%s\n'   timer.o | awk '!x[$$0]++ { print("/home/dell/device/27-1/"$$0) }' > /home/dell/device/27-1/timer.mod
