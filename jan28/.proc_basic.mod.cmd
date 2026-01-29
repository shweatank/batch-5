savedcmd_/home/dell/practice/jan28/proc_basic.mod := printf '%s\n'   proc_basic.o | awk '!x[$$0]++ { print("/home/dell/practice/jan28/"$$0) }' > /home/dell/practice/jan28/proc_basic.mod
