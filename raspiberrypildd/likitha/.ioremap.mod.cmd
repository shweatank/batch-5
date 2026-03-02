savedcmd_/home/team2/likitha/ioremap.mod := printf '%s\n'   ioremap.o | awk '!x[$$0]++ { print("/home/team2/likitha/"$$0) }' > /home/team2/likitha/ioremap.mod
