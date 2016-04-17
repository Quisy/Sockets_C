#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<math.h>
#include<time.h>

#define SQRT_IN 1000
#define TIME_IN 1002

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

void littleToBigEndianI(int *val)
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

void littleToBigEndianF(float *val)
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

void littleToBigEndianS(char str[])
{
    // if(checkEndian() == LITTLE_END)
    // {
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
    // }
}


char* getCurrentDateTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    static char timeStr[DATETIME_SIZE+1];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(timeStr, DATETIME_SIZE+1, "%Y:%m:%d %H:%M:%S", timeinfo);
    return timeStr;
}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    const int backlog = 3;
    int requestType;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , backlog);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    while((read_size = recv(client_sock , &requestType , sizeof(requestType) , 0)) > 0 )
    {
        littleToBigEndianI(&requestType);
        if(requestType == SQRT_IN)
        {
            float number, result;
            puts("SQRT REQUEST");
            
            if((read_size = recv(client_sock , &number , sizeof(number) , 0)) > 0 )
            {
                littleToBigEndianF(&number);
                printf("Number: %f\n", number);
                result = sqrt(number);
                printf("Result: %f\n", result);
                littleToBigEndianF(&result);
                write(client_sock , &result , sizeof(result));
            }
        }
        else if(requestType == TIME_IN)
        {
            puts("TIME REQUEST");
            char* time;
            time = getCurrentDateTime();
            printf ( "Current local time and date: %s\n", time);
            littleToBigEndianS(time);
            write(client_sock , time , sizeof(char)* DATETIME_SIZE);
        }
        else
        {
            puts("UNKNOWN REQUEST");
        }
        //Send the message back to client
        //write(client_sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}