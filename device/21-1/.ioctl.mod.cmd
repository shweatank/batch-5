savedcmd_/home/dell/device/21-1/ioctl.mod := printf '%s\n'   ioctl.o | awk '!x[$$0]++ { print("/home/dell/device/21-1/"$$0) }' > /home/dell/device/21-1/ioctl.mod
