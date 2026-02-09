savedcmd_/home/mirafra/kernel_tcp/kclient.mod := printf '%s\n'   kclient.o | awk '!x[$$0]++ { print("/home/mirafra/kernel_tcp/"$$0) }' > /home/mirafra/kernel_tcp/kclient.mod
