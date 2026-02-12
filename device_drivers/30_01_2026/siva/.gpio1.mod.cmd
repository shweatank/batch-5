savedcmd_/home/team1/siva/gpio1.mod := printf '%s\n'   gpio1.o | awk '!x[$$0]++ { print("/home/team1/siva/"$$0) }' > /home/team1/siva/gpio1.mod
