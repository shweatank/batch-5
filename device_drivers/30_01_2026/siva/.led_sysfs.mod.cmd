savedcmd_/home/team1/siva/led_sysfs.mod := printf '%s\n'   led_sysfs.o | awk '!x[$$0]++ { print("/home/team1/siva/"$$0) }' > /home/team1/siva/led_sysfs.mod
