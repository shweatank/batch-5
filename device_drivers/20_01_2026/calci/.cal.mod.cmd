savedcmd_/home/dell/practice/20_01_2026/calci/cal.mod := printf '%s\n'   cal.o | awk '!x[$$0]++ { print("/home/dell/practice/20_01_2026/calci/"$$0) }' > /home/dell/practice/20_01_2026/calci/cal.mod
