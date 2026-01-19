savedcmd_basic_char.mod := printf '%s\n'   basic_char.o | awk '!x[$$0]++ { print("./"$$0) }' > basic_char.mod
