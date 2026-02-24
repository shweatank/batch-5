savedcmd_/home/dell/practice/basic_char.mod := printf '%s\n'   basic_char.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/basic_char.mod
