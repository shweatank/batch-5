savedcmd_/home/team1/sai/key_board_irq.mod := printf '%s\n'   key_board_irq.o | awk '!x[$$0]++ { print("/home/team1/sai/"$$0) }' > /home/team1/sai/key_board_irq.mod
