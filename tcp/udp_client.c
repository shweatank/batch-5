/************* UDP CLIENT CODE *******************/

#include"headers.h"
int main(int argc,char **argv)
{
        int clientSocket, portNum, nBytes;
        char buffer[1024];
        struct sockaddr_in serverAddr;
        socklen_t addr_size;

        if(argc<3)
        {
                puts("input:./udpCli(exe) server_port server_ip\n");
                return 0;
        }

        /*Create UDP socket*/
        clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

        /*Configure settings in address struct*/
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((unsigned short)atoi(argv[1]));
        serverAddr.sin_addr.s_addr = inet_addr(argv[2]);

        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

        /*Initialize size variable to be used later on*/
        addr_size = sizeof serverAddr;

        while(1)
        {
                printf("Enter message to send to server:\n");
                fgets(buffer,1024,stdin);
                if(buffer[strlen(buffer)-1]==10)
                        buffer[strlen(buffer)-1]=0;

                printf("You typed: %s\n",buffer);
                nBytes = strlen(buffer) + 1;

                /*Send message to server*/
                sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);

                /*Receive message from server*/
                nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);

                printf("Received from server: %s\n",buffer);

        }

        return 0;
}

