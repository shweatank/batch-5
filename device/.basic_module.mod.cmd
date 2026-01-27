savedcmd_/home/dell/device/basic_module.mod := printf '%s\n'   basic_module.o | awk '!x[$$0]++ { print("/home/dell/device/"$$0) }' > /home/dell/device/basic_module.mod
