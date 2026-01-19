savedcmd_basic_char_driver.mod := printf '%s\n'   basic_char_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > basic_char_driver.mod
