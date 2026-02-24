savedcmd_/home/team1/praneeth/gpio_status_reflect.mod := printf '%s\n'   gpio_status_reflect.o | awk '!x[$$0]++ { print("/home/team1/praneeth/"$$0) }' > /home/team1/praneeth/gpio_status_reflect.mod
