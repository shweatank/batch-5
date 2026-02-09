savedcmd_/home/mirafra/basic_module/basic_module.mod := printf '%s\n'   basic_module.o | awk '!x[$$0]++ { print("/home/mirafra/basic_module/"$$0) }' > /home/mirafra/basic_module/basic_module.mod
