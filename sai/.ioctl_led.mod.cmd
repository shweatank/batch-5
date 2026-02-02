savedcmd_/home/team1/sai/ioctl_led.mod := printf '%s\n'   ioctl_led.o | awk '!x[$$0]++ { print("/home/team1/sai/"$$0) }' > /home/team1/sai/ioctl_led.mod
