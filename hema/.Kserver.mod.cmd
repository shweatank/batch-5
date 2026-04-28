savedcmd_/home/team2/hema/Kserver.mod := printf '%s\n'   Kserver.o | awk '!x[$$0]++ { print("/home/team2/hema/"$$0) }' > /home/team2/hema/Kserver.mod
