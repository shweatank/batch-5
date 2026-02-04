savedcmd_/home/mirafra/Desktop/batch-5/UART/uart.mod := printf '%s\n'   uart.o | awk '!x[$$0]++ { print("/home/mirafra/Desktop/batch-5/UART/"$$0) }' > /home/mirafra/Desktop/batch-5/UART/uart.mod
