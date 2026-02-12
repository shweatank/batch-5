savedcmd_/home/team1/siva/kthread.mod := printf '%s\n'   kthread.o | awk '!x[$$0]++ { print("/home/team1/siva/"$$0) }' > /home/team1/siva/kthread.mod
