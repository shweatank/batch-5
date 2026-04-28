savedcmd_/home/team2/hema/detect.mod := printf '%s\n'   detect.o | awk '!x[$$0]++ { print("/home/team2/hema/"$$0) }' > /home/team2/hema/detect.mod
