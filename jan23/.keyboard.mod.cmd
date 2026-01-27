savedcmd_/home/dell/practice/jan23/keyboard.mod := printf '%s\n'   keyboard.o | awk '!x[$$0]++ { print("/home/dell/practice/jan23/"$$0) }' > /home/dell/practice/jan23/keyboard.mod
