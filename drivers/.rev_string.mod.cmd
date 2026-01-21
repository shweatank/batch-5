savedcmd_/home/dell/mirafra/drivers/rev_string.mod := printf '%s\n'   rev_string.o | awk '!x[$$0]++ { print("/home/dell/mirafra/drivers/"$$0) }' > /home/dell/mirafra/drivers/rev_string.mod
