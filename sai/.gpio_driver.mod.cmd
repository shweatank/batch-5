savedcmd_/home/team1/sai/gpio_driver.mod := printf '%s\n'   gpio_driver.o | awk '!x[$$0]++ { print("/home/team1/sai/"$$0) }' > /home/team1/sai/gpio_driver.mod
