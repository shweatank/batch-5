savedcmd_/home/team2/multimedia/viedo/lefttoright.mod := printf '%s\n'   lefttoright.o | awk '!x[$$0]++ { print("/home/team2/multimedia/viedo/"$$0) }' > /home/team2/multimedia/viedo/lefttoright.mod
