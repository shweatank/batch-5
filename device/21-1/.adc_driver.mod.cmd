savedcmd_/home/dell/device/21-1/adc_driver.mod := printf '%s\n'   adc_driver.o | awk '!x[$$0]++ { print("/home/dell/device/21-1/"$$0) }' > /home/dell/device/21-1/adc_driver.mod
