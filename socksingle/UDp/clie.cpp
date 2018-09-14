#include <iostream>
#include <stdio.h>
//#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include<fstream>
using namespace std;
int main(int argc, char ** argv)                                   // taking cli input
{

        if(argc != 2)
        {
                cerr << "Usage: port" << endl;                     //if port no is not provided
                exit(0); 
        }
	
	int portno = atoi(argv[1]), connection_flag = 0;
        //port number to integer
        int network_socket;

	char buffer[1024]={0};
        //buffer for sending and receiving messgaes 
        
        struct sockaddr_in server_address;
        network_socket = socket(AF_INET , SOCK_DGRAM , 0 );
        memset(&server_address, 0, sizeof(server_address));
        //structure for socket addrress

        server_address.sin_family = AF_INET;
        //TCP family

        server_address.sin_port = htons(portno);                            
        //address details

        server_address.sin_addr.s_addr = INADDR_ANY;
        // Address ip

        
        //socket connection
        if(network_socket < 0)                             //failed in initializing socket
	    {
            cerr<<"Error establishing the server socket"<<endl;    
		    //unable to connect
            exit(0);
        }
        
        cout<<"\nConnected to server....\n"<<endl;
        //loop for making infinite requests untill client types exit.
        while(1)
        {
            
            //ofstream out( "textfile.txt" );
            //out<< "busy";
            //out.close();
	        cout<<"Enter your Expression:\n> ";	
	        string str="";
	        cin>>str;
    	        char *hello = &str[0u];                        
                //converting string to char*

                if(str == "exit" || str == "shut")
                {
                        sendto(network_socket, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &server_address,sizeof(server_address));              
                        //sending exit command to server;
                        break;
                }

	        sendto(network_socket, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &server_address,sizeof(server_address));              
                                        //sending expression to calculate to server
	        //cout<<"Hello Message sent\n";	

                cout<<"\n\nWaiting for reply....\n";
                char server_message[256];

            int valread;

        	valread = recvfrom(network_socket, (char *)server_message, 256,MSG_WAITALL, (struct sockaddr *) &server_address,(socklen_t*)&server_address);
            buffer[valread] = '\0';
            printf("Server : %s\n", server_message);                             //receiving answer of expression from server
                cout<<"val rread = "<<valread;
                if(!strcmp(buffer,"exit") || valread == 0 || valread ==-1)
                {
                        cout<<"\n\nYour Seesion has been ended by server\n\n";                //session ended if server reply exit
                        break;
                }
        	printf("\nCalculated Answer is: %s\n",server_message);                                         // answer of the calculation
                        
        }
        close(network_socket);           //closing client socket
        return 0;

}