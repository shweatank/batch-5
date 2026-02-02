savedcmd_/home/team1/sai/basic_char.mod := printf '%s\n'   basic_char.o | awk '!x[$$0]++ { print("/home/team1/sai/"$$0) }' > /home/team1/sai/basic_char.mod
