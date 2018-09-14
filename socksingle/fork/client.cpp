#include<sys/socket.h>
#include<sys/types.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<string>
using namespace std;
int main(int argv , char** argc)
{
    if(argv < 2)
    {
        perror("Ussage : PORT");
        exit(EXIT_FAILURE);
    }
    int portno = atoi(argc[1]);

    struct sockaddr_in client_address;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(portno);

    int sockfd = socket( AF_INET,SOCK_STREAM , 0);
    if(sockfd < 0)
    {
        perror("Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    int connect_status = connect(sockfd , (struct sockaddr*)&client_address, sizeof(client_address));
    if(connect_status < 0)
    {
        perror("Unable to connect to the server\n");
        exit(EXIT_FAILURE);
    }
    cout<<"Connected to server\n";
    cout<<"Enter Your Expression.\n>";
    string str="";
	cin>>str;
    char *hello = &str[0u];                        
    //converting string to char*
    if(str == "exit" || str == "shut")
    {
        send(sockfd,hello, strlen(hello), 0);               
        //sending exit command to server;
        close(sockfd);
        exit(0);
    }
    send(sockfd,hello, strlen(hello), 0);

    cout<<"\n\nWaiting for reply....\n";
                
    char server_message[256];
    int valread = read(sockfd , server_message , 1024);                             //receiving answer of expression from server
    cout<<"val rread = "<<valread;
    if(!strcmp(server_message,"exit") || valread == 0 || valread ==-1)
    {
        cout<<"\n\nYour Seesion has been ended by server\n\n";                //session ended if server reply exit
        close(sockfd);
        exit(0);
    }
    printf("\nCalculated Answer is: %s\n",server_message);
    close(sockfd);
    return 0;
}