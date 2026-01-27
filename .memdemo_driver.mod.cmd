savedcmd_/home/dell/practice/memdemo_driver.mod := printf '%s\n'   memdemo_driver.o | awk '!x[$$0]++ { print("/home/dell/practice/"$$0) }' > /home/dell/practice/memdemo_driver.mod
