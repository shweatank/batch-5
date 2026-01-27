savedcmd_/home/dell/device/23-1/char_device.mod := printf '%s\n'   char_device.o | awk '!x[$$0]++ { print("/home/dell/device/23-1/"$$0) }' > /home/dell/device/23-1/char_device.mod
