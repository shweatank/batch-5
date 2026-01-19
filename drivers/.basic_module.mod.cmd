savedcmd_basic_module.mod := printf '%s\n'   basic_module.o | awk '!x[$$0]++ { print("./"$$0) }' > basic_module.mod
