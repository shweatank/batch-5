savedcmd_/home/team2/likitha/sysfs.mod := printf '%s\n'   sysfs.o | awk '!x[$$0]++ { print("/home/team2/likitha/"$$0) }' > /home/team2/likitha/sysfs.mod
