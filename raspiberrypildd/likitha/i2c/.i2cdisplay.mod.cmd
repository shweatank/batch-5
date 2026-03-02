savedcmd_/home/team2/likitha/i2c/i2cdisplay.mod := printf '%s\n'   i2cdisplay.o | awk '!x[$$0]++ { print("/home/team2/likitha/i2c/"$$0) }' > /home/team2/likitha/i2c/i2cdisplay.mod
