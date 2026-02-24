savedcmd_/home/dell/rpi_cross_driver/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/dell/rpi_cross_driver/"$$0) }' > /home/dell/rpi_cross_driver/hello.mod
