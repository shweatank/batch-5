savedcmd_/home/dell/mirafra/drivers/basic_char.mod := printf '%s\n'   basic_char.o | awk '!x[$$0]++ { print("/home/dell/mirafra/drivers/"$$0) }' > /home/dell/mirafra/drivers/basic_char.mod
