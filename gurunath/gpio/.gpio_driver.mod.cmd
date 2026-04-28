savedcmd_/home/team2/gurunath/gpio/gpio_driver.mod := printf '%s\n'   gpio_driver.o | awk '!x[$$0]++ { print("/home/team2/gurunath/gpio/"$$0) }' > /home/team2/gurunath/gpio/gpio_driver.mod
