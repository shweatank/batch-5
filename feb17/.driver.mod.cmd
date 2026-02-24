savedcmd_/home/dell/practice/feb17/driver.mod := printf '%s\n'   driver.o | awk '!x[$$0]++ { print("/home/dell/practice/feb17/"$$0) }' > /home/dell/practice/feb17/driver.mod
