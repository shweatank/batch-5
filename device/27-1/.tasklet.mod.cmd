savedcmd_/home/dell/device/27-1/tasklet.mod := printf '%s\n'   tasklet.o | awk '!x[$$0]++ { print("/home/dell/device/27-1/"$$0) }' > /home/dell/device/27-1/tasklet.mod
