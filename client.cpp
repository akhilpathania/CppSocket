#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<signal.h>
using namespace std;
int clientSocket, conn_stat;
void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;
    close(clientSocket);
    kill(getppid(),SIGQUIT); kill(getpid(),SIGQUIT);
    exit(0);
}
int PORT;

int main(int argv , char **argc)
{
    if(argv < 2 )
    {
        perror("Ussage: Port");
        exit(0);
    }
    PORT = atoi(argc[1]);
	
	struct sockaddr_in serverAddr,clientAddr;
	char buffer[1024];


	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
    {
		perror("Error in connection.\n");
		exit(1);
	}
	cout<<"Client Socket is created.\n";

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    clientAddr.sin_addr.s_addr = inet_addr("10.0.2.15");
    clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(12000);

    if (bind(clientSocket, (struct sockaddr*) &clientAddr, sizeof(struct sockaddr_in)) == 0)
        printf("Binded Correctly\n");
    else
        printf("Unable to bind\n");
	

	conn_stat = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(conn_stat < 0)
    {
		perror("Error in connection.\n");
		exit(1);
	}
	cout<<"Connected to Server.\n"<<endl;

	while(1)
    {

         cout<<"Enter Your Expression.\n>";
        //cout<<"Waiting Client Request...."<<endl;
        signal(SIGABRT, &sighandler);
	    signal(SIGTERM, &sighandler);
	    signal(SIGINT, &sighandler);
        
        string str="";
	    cin>>str;
        char *hello = &str[0u];
                             
        //converting string to char*
        if(str == "exit" || str == "shut")
        {
            send(clientSocket,hello, strlen(hello), 0);
            cout<<"Disconnected from server.\n";               
            //sending exit command to server;
            close(clientSocket);
            kill(getppid(),SIGQUIT);
            exit(1);
        }
        send(clientSocket,hello, strlen(hello), 0);

        cout<<"\n\nWaiting for reply....\n";
		

		char server_message[256];
        memset(server_message , 0 , 256);
        int valread = read(clientSocket , server_message , 1024);                             //receiving answer of expression from server
        if(!strcmp(server_message,"exit") || valread == 0 || valread ==-1)
        {
            cout<<"\n\nYour Seesion has been ended by server\n\n";                //session ended if server reply exit
            close(clientSocket);
            exit(0);
        }
        cout<<"\nCalculated Answer is: "<<server_message<<endl<<endl;
        cout<<"-----------------------------------------------"<<endl<<endl;
	}
    close(clientSocket);
	return 0;
}