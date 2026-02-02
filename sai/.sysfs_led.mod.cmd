savedcmd_/home/team1/sai/sysfs_led.mod := printf '%s\n'   sysfs_led.o | awk '!x[$$0]++ { print("/home/team1/sai/"$$0) }' > /home/team1/sai/sysfs_led.mod
