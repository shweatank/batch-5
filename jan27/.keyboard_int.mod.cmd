savedcmd_/home/dell/practice/jan27/keyboard_int.mod := printf '%s\n'   keyboard_int.o | awk '!x[$$0]++ { print("/home/dell/practice/jan27/"$$0) }' > /home/dell/practice/jan27/keyboard_int.mod
