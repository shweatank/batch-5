savedcmd_/home/mirafra/Desktop/batch-5/proj/proj.mod := printf '%s\n'   proj.o | awk '!x[$$0]++ { print("/home/mirafra/Desktop/batch-5/proj/"$$0) }' > /home/mirafra/Desktop/batch-5/proj/proj.mod
