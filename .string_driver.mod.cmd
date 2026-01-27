savedcmd_/home/dell/practice/string_driver.mod := printf '%s\n'   string_driver.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/string_driver.mod
