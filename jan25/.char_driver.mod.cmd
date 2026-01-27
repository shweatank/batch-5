savedcmd_/home/dell/practice/jan25/char_driver.mod := printf '%s\n'   char_driver.o | awk '!x[$$0]++ { print("/home/dell/practice/jan25/"$$0) }' > /home/dell/practice/jan25/char_driver.mod
