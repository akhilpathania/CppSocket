// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<iostream>
using namespace std;
 

#define MAXLINE 1024
 
// Driver code
int main(int argc, char ** argv)                                   // taking cli input
{

        if(argc != 2)
        {
                cerr << "Usage: port" << endl;                     //if port no is not provided
                exit(0); 
        }
	
	int PORT = atoi(argv[1]), connection_flag = 0;
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
 
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
     
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while(1)
    {
            int n, len;
        cout<<"Enter your Expression:\n> ";	
	    string str="";
	    cin>>str;
        hello = &str[0u];                        
                //converting string to char*
        if(str == "exit" || str == "shut")
        {
            sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM,(const struct sockaddr *)&servaddr,sizeof(servaddr));
            cout<<"You quited session\n";
            break;
        }

        sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM,(const struct sockaddr *)&servaddr,sizeof(servaddr));
        cout<<"Waiting for server to reply\n";
         
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,(socklen_t*)&len);
        buffer[n] = '\0';
        

        cout<<"Answer is: "<<buffer<<endl;
 
    } 
    
    close(sockfd);
    return 0;
}