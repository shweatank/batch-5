savedcmd_/home/dell/device/char_device.mod := printf '%s\n'   char_device.o | awk '!x[$$0]++ { print("/home/dell/device/"$$0) }' > /home/dell/device/char_device.mod
