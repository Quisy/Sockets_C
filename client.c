#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<stdlib.h>
 
#define SQRT_IN 2000
#define TIME_IN 2002 
 
#define SQRT_OUT 1000
#define TIME_OUT 1002 

#define BIG_END 1
#define LITTLE_END 0

#define DATETIME_SIZE 19
 
int checkEndian()
{
	int i = 1;
	char *d = (char*)&i;

	if(d[0] == 1)
		return BIG_END;
	else
		return LITTLE_END;
}

void endianConvertInt(int *val)
{
    if(checkEndian() == LITTLE_END)
    {
        int i = 0, k=0;
        int currentValTemp = *val;
        char *old = (char*)&currentValTemp;
        char *new = (char*)val;
        
        for(i; i<4; i++)
        {
           new[i] = old[3-i]; 
        }
        
        for(k; k<4; k++)
        {
           printf("%d  %d\n",(int)new[k],(int)old[k]);
        }
    }
}

void endianConvertFloat(float *val)
{
    if(checkEndian() == LITTLE_END)
    {
        int i = 0, k=0;
        int currentValTemp = *val;
        char *old = (char*)&currentValTemp;
        char *new = (char*)val;
        
        for(i; i<4; i++)
        {
           new[i] = old[3-i]; 
        }
        
        for(k; k<4; k++)
        {
           printf("%d  %d\n",(int)new[k],(int)old[k]);
        }
    }
}

void endianConvertString(char str[])
{
    if(checkEndian() == LITTLE_END)
    {
        int size = DATETIME_SIZE;
        int i = 0;
        char *s = str;
        char temp;
        
        
        for(i; i<(size/2); i++)
        {
            temp = s[i];  
            printf("%c   ",s[i]); 
            s[i] = s[size - 1 - i];
            printf("%c\n",s[i]); 
            s[size - 1 - i] = temp;
        }
        // while(*s++ != '\0')
        // {
        //     temp = *s;           
        // }
        printf("%s\n",s);                
    }
}
 
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
         
        endianConvertInt(&requestType); 
         
        //Send some data
        if( send(sock , &requestType , sizeof(requestType) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        
        if(recv(sock , &requestType , sizeof(requestType) , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
        endianConvertInt(&requestType); 
         
        if(requestType == SQRT_IN)
        {
            float number, result;
            puts("Number:");
            scanf("%f", &number);
            
            endianConvertFloat(&number);
            
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
            
            endianConvertFloat(&result);
            printf("Result: %f\n", result);
        } 
        else if(requestType = TIME_IN)
        {
            char* time;
            int size = DATETIME_SIZE;
            time = malloc(sizeof(char)* size);
            if(recv(sock , time , sizeof(char)*size , 0) < 0)
        	{
            	puts("recv failed");
            	break;
        	}
            endianConvertString(time);
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
