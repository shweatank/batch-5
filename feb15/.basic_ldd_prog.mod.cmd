savedcmd_/home/dell/practice/feb15/basic_ldd_prog.mod := printf '%s\n'   basic_ldd_prog.o | awk '!x[$$0]++ { print("/home/dell/practice/feb15/"$$0) }' > /home/dell/practice/feb15/basic_ldd_prog.mod
