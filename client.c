#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
#define SQUARE_OUT 1000
#define TIME_OUT 1002 
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];
    int requestType;
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8080 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        printf("Request type: ");
        scanf("%d", &requestType);
         
        //Send some data
        if( send(sock , &requestType , sizeof(requestType) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        // if( recv(sock , server_reply , 2000 , 0) < 0)
        // {
        //     puts("recv failed");
        //     break;
        // }
         
        //puts("Server reply :");
        //puts(server_reply);
    }
     
    close(sock);
    return 0;
}