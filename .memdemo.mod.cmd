savedcmd_/home/mirafra/Desktop/batch-5/memdemo.mod := printf '%s\n'   memdemo.o | awk '!x[$$0]++ { print("/home/mirafra/Desktop/batch-5/"$$0) }' > /home/mirafra/Desktop/batch-5/memdemo.mod
