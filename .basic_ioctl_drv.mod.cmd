savedcmd_/home/dell/practice/basic_ioctl_drv.mod := printf '%s\n'   basic_ioctl_drv.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/basic_ioctl_drv.mod
