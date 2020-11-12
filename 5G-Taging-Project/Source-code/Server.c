#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#define PORT 8080


int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char mess_from_client[225];
    char buffer1[1024] = {0};
    char buffer2[1024] = {0};
    char buffer3[1024] = {0};
    char buffer4[1024] = {0};
    char buffer5[1024] = {0};

    char *hello = "----Hello from Server----";
    int continu = 1;
    //tao socket
    // tao file mo ta soket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //gan dia chi cho socket
    // gan cong port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );  //gan cong la 8080
    // bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //listen, chi dinh socket lang nghe ket noi
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //accept, chap nhan ket noi
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char str_cli_ip[INET_ADDRSTRLEN];
    struct sockaddr_in* ip_client = (struct sockaddr_in*)&address;
    inet_ntop(AF_INET, &ip_client->sin_addr, str_cli_ip, INET_ADDRSTRLEN);
    printf("ipclient: %s\n", str_cli_ip );
    char str_cli_port[INET_ADDRSTRLEN];
    printf("port: %d\n", ntohs(ip_client->sin_port));
    //read, doc du lieu gan vao bien valread tra ve so byte ma no doc duoc

    // Receive data from AMF
    int numberOfPackets = 5;

    // Queue of UE-ID ith
    char queue[5][15];
    int i = 0;
    while(numberOfPackets--){

        printf("* Message Type field from client: \n");
        valread = read( new_socket, buffer1, 1024);
        hello = &buffer1;
        printf("%s\n",buffer1 );
        send(new_socket, hello, strlen(hello), 0 );

        printf("* UE ID from client: \n");
        valread = read( new_socket, buffer2, 1024);
        hello = &buffer2;
        printf("%s\n",buffer2 );
        send(new_socket, hello, strlen(hello), 0 );
        // Push UE_IDs value to queue
        strcpy(queue[i], buffer2);
        i++;

        printf("* TAC from client: \n");
        valread = read( new_socket, buffer3, 1024);
        hello = &buffer3;
        printf("%s\n",buffer3 );
        send(new_socket, hello, strlen(hello), 0 );

        printf("* CN Domain from client: \n");
        valread = read( new_socket, buffer4, 1024);
        hello = &buffer4;
        printf("%s\n",buffer4 );
        send(new_socket, hello, strlen(hello), 0 );
    }

    // Receive from UEs and sent Data to UEs
    int j = 0;
    int check = 1;
    while(check && j < 5){
        printf("* UE waked up: \n");
        valread = read( new_socket, buffer5, 1024);
        hello = &buffer5;
        printf("%s\n",buffer5 );

        if(strcmp(buffer1, "100") == 0 && strcmp(buffer3, "100") == 0 && strcmp(buffer4, "100") == 0 && strcmp(buffer5, "True") == 0){
                send(new_socket, &queue[j], strlen((&queue[j])), 0 );
                j++;
        }
    }

    // Kiem tra dieu kien
    close(new_socket);
    return 0;
}
