#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

// AMF
struct Packets {
    char *messageType, *TAC, *CN_Domain, *WakeUp;
    int UE_ID;
};

int main(int argc, char const *argv[])
{
    struct Packets p;
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "----Hello from Client----";
    char buffer1[1024] = {0};
    char buffer2[1024] = {0};
    char buffer3[1024] = {0};
    char buffer4[1024] = {0};
    char buffer5[1024] = {0};

    char add[225];
    int continu = 1;
    printf("- Enter the IP addr of gNodeB: \n");
    gets(add);
    //tao socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, add, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    // connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // The nunmber of packets
    int numberOfPackets = 5;
    while(numberOfPackets--){
        // Initilize fields
        p.messageType = "100";
        char *str[1000];
        int lower = 99, upper = 1000;
        p.UE_ID = rand() % (upper - lower + 1) + lower;
        sprintf(str, "%d", p.UE_ID);
        p.TAC = "100";
        p.CN_Domain = "100";

        /* AMF */
        send(sock , p.messageType , strlen(p.messageType) , 0 );
        valread = read( sock , buffer1, 1024);
        send(sock , str , strlen(str) , 0 );
        valread = read( sock , buffer2, 1024);
        send(sock , p.TAC , strlen(p.TAC) , 0 );
        valread = read( sock , buffer3, 1024);
        send(sock , p.CN_Domain , strlen(p.CN_Domain) , 0 );
        valread = read( sock , buffer4, 1024);
    }


    /* Client 2 - UEs*/
    int t = 0;
    p.WakeUp = "True";
    while(1){
        sleep(1);
        t += 1;
        if(t % 4 == 0){
            send(sock , p.WakeUp , strlen(p.WakeUp ) , 0 );
            printf("*** UE received successfully UE-ID from gNodeB *** \n");
            valread = read( sock , buffer5, 1024);
            printf("%s\n",buffer5);
        }

    }

    //close(sock);
    return 0;
}
