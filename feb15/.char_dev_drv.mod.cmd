savedcmd_/home/dell/practice/feb15/char_dev_drv.mod := printf '%s\n'   char_dev_drv.o | awk '!x[$$0]++ { print("/home/dell/practice/feb15/"$$0) }' > /home/dell/practice/feb15/char_dev_drv.mod
