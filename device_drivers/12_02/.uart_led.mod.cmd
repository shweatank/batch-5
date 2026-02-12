savedcmd_/home/team1/siva/12_02/uart_led.mod := printf '%s\n'   uart_led.o | awk '!x[$$0]++ { print("/home/team1/siva/12_02/"$$0) }' > /home/team1/siva/12_02/uart_led.mod
