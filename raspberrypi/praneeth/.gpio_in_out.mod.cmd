savedcmd_/home/team1/praneeth/gpio_in_out.mod := printf '%s\n'   gpio_in_out.o | awk '!x[$$0]++ { print("/home/team1/praneeth/"$$0) }' > /home/team1/praneeth/gpio_in_out.mod
