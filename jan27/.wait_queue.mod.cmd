savedcmd_/home/dell/practice/jan27/wait_queue.mod := printf '%s\n'   wait_queue.o | awk '!x[$$0]++ { print("/home/dell/practice/jan27/"$$0) }' > /home/dell/practice/jan27/wait_queue.mod
