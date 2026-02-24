savedcmd_/home/dell/practice/feb16/kern_client.mod := printf '%s\n'   kern_client.o | awk '!x[$$0]++ { print("/home/dell/practice/feb16/"$$0) }' > /home/dell/practice/feb16/kern_client.mod
