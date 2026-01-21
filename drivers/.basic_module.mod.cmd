savedcmd_/home/dell/mirafra/drivers/basic_module.mod := printf '%s\n'   basic_module.o | awk '!x[$$0]++ { print("/home/dell/mirafra/drivers/"$$0) }' > /home/dell/mirafra/drivers/basic_module.mod
