savedcmd_/home/dell/practice/jan28/sysfs_driver.mod := printf '%s\n'   sysfs_driver.o | awk '!x[$$0]++ { print("/home/dell/practice/jan28/"$$0) }' > /home/dell/practice/jan28/sysfs_driver.mod
