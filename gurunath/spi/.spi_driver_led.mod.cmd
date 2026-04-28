savedcmd_/home/team2/gurunath/spi/spi_driver_led.mod := printf '%s\n'   spi_driver_led.o | awk '!x[$$0]++ { print("/home/team2/gurunath/spi/"$$0) }' > /home/team2/gurunath/spi/spi_driver_led.mod
