savedcmd_/home/team2/hema/i2c_rtc.mod := printf '%s\n'   i2c_rtc.o | awk '!x[$$0]++ { print("/home/team2/hema/"$$0) }' > /home/team2/hema/i2c_rtc.mod
