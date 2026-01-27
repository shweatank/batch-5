savedcmd_/home/dell/device/char_basic.mod := printf '%s\n'   char_basic.o | awk '!x[$$0]++ { print("/home/dell/device/"$$0) }' > /home/dell/device/char_basic.mod
