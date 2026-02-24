savedcmd_/home/dell/mirafra/tcp_ip/Kernel_server.mod := printf '%s\n'   Kernel_server.o | awk '!x[$$0]++ { print("/home/dell/mirafra/tcp_ip/"$$0) }' > /home/dell/mirafra/tcp_ip/Kernel_server.mod
