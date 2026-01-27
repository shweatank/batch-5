savedcmd_/home/dell/device/23-1/key_press.mod := printf '%s\n'   key_press.o | awk '!x[$$0]++ { print("/home/dell/device/23-1/"$$0) }' > /home/dell/device/23-1/key_press.mod
