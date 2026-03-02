savedcmd_/home/team2/likitha/dts_blink.mod := printf '%s\n'   dts_blink.o | awk '!x[$$0]++ { print("/home/team2/likitha/"$$0) }' > /home/team2/likitha/dts_blink.mod
