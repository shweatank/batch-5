savedcmd_/home/team2/gurunath/dts/dts_driver.mod := printf '%s\n'   dts_driver.o | awk '!x[$$0]++ { print("/home/team2/gurunath/dts/"$$0) }' > /home/team2/gurunath/dts/dts_driver.mod
