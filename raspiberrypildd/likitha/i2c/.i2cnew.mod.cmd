savedcmd_/home/team2/likitha/i2c/i2cnew.mod := printf '%s\n'   i2cnew.o | awk '!x[$$0]++ { print("/home/team2/likitha/i2c/"$$0) }' > /home/team2/likitha/i2c/i2cnew.mod
