savedcmd_/home/dell/practice/jan28/proc_int_wq.mod := printf '%s\n'   proc_int_wq.o | awk '!x[$$0]++ { print("/home/dell/practice/jan28/"$$0) }' > /home/dell/practice/jan28/proc_int_wq.mod
