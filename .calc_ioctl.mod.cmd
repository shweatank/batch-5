savedcmd_/home/dell/practice/calc_ioctl.mod := printf '%s\n'   calc_ioctl.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/calc_ioctl.mod
