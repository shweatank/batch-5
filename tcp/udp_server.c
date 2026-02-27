/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include<arpa/inet.h>

int palindrome(char *p)
{
        int j=0,i=strlen(p)-1;
        while(j<i)
                if(p[i]!=p[j])
                        return 0;
                else
                        i--,j++;

        return 1;

}



int main(int argc,char **argv)
{
        int udpSocket, nBytes;
        char buffer[100];
        struct sockaddr_in serverAddr, clientAddr;
        socklen_t addr_size;

        if(argc<2)
        {
                puts("input:./udpSer(exe) server_port");
                return 0;
        }

        /*Create UDP socket*/
        udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

        /*Configure settings in address struct*/
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(atoi(argv[1]));
        serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

        /*Bind socket with address struct*/
        bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

        /*Initialize size variable to be used later on*/
        addr_size = sizeof(clientAddr);

        while(1)
        {
                printf("udp server waiting on port no: %s\n",argv[1]);
                nBytes = recvfrom(udpSocket,buffer,100,0,(struct sockaddr *)&clientAddr, &addr_size);

                unsigned short portNo=(unsigned short)ntohs(clientAddr.sin_port);
                char *ip=inet_ntoa(clientAddr.sin_addr);
                printf("got message from client:\n");
                printf("Using port: %d\n",portNo);
                printf("With ip: %s\n",ip);

                if(nBytes<0)
                {
                        perror("recvfrom");
                        break;
                }
                else if(nBytes==0)
                {
                        puts("client closed abruptly");
                        break;
                }
                else
                {
                        printf("server recvd: %s\n",buffer);
                }
                puts("And server echoed\n");


                int x=palindrome(buffer);


                if(x==1)
                {

                        /*Send uppercase message back to client, using serverStorage as the address*/
                        sendto(udpSocket,"palindrome",11,0,(struct sockaddr *)&clientAddr,addr_size);
                }
                else
                        sendto(udpSocket,"Not palindrome",15,0,(struct sockaddr *)&clientAddr,addr_size);


        }

        return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
