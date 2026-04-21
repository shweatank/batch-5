savedcmd_/home/team2/gurunath/i2c/i2c_o_led.mod := printf '%s\n'   i2c_o_led.o | awk '!x[$$0]++ { print("/home/team2/gurunath/i2c/"$$0) }' > /home/team2/gurunath/i2c/i2c_o_led.mod
