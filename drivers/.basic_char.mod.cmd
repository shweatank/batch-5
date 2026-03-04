savedcmd_/home/mirafra/batch-5/drivers/basic_char.mod := printf '%s\n'   basic_char.o | awk '!x[$$0]++ { print("/home/mirafra/batch-5/drivers/"$$0) }' > /home/mirafra/batch-5/drivers/basic_char.mod
