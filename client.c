#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<stdlib.h>
 
#define SQRT_OUT 1000
#define TIME_OUT 1002 

#define DATETIME_SIZE 19
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
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
         
        if(requestType == SQRT_OUT)
        {
            float number, result;
            puts("Number:");
            scanf("%f", &number);
            
            if( send(sock , &number , sizeof(number) , 0) < 0)
            {
                puts("Send failed");
                return 1;
            }
            
            puts("SERVER MESSAGE: ");
            if(recv(sock , &result , sizeof(result) , 0) < 0)
        	{
            	puts("recv failed");
            	break;
        	}
            printf("Result: %f\n", result);
        } 
        else if(requestType = TIME_OUT)
        {
            char* time;
            int size = DATETIME_SIZE;
            time = malloc(sizeof(char)* size);
            if(recv(sock , time , sizeof(char)*size , 0) < 0)
        	{
            	puts("recv failed");
            	break;
        	}
            printf("Current Date and Time: %s\n", time);
            puts(time);
            free(time);
            
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
