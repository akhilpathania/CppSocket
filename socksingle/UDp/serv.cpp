#include<stdio.h>    
#include<iostream>
#include<stdlib.h>
//#include<sys/socket.h>                  // for socket fuctions
#include<netinet/in.h>                  // for address family
#include <unistd.h>
#include<string.h>
#include <sstream>	
#include<signal.h>					// string stream header		
#include<errno.h>		            // Error no header	
using namespace std;
int portno;

//function for closing commands
void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;
    exit(0);
}

//**************8socket function
void socketinit(int port)
{
    char buffer[1024] = {0};
	int server_socket;
    //int max_clients = 30 , activity, i , valread , sd;  
	//fd_set readfds; 

    int i=1;
	//structure for getting server address
	struct sockaddr_in server_address,client_address;   							 
	//server information
	int addrlen = sizeof(server_address);

    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address)); 

	server_address.sin_family = AF_INET;           					
	// server type    here TCP

	server_address.sin_port = htons(port);
	//assigning port number & also converting int port to xact type needed using htons
    int client_socket;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);      		
	// local server address
    
	server_socket = socket(AF_INET , SOCK_DGRAM , 0);       // creating & initializing socket  			
	
	if(server_socket < 0)                             //failed in initializing socket
	{
        cerr<<"Error establishing the server socket"<<endl;    
		//unable to connect
        
        exit(0);
    }
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));

	int bind_stat = bind(server_socket , (struct sockaddr *)&server_address , sizeof(server_address));     
	//bind port and address
	
	if(bind_stat < 0)                   // unable to bind the server address data conditioon.
	{
		cerr<<"Error Binding socket"<<endl;        	
			//unable to bind port
            cout<<"\nBinding error with number "<<errno<<endl<<strerror(errno)<<endl;
		exit(0);
	}
    
	cout<<"Waiting for clients to connect..."<<endl;
    
    
    while(1)                                       								
	//after successfull connecting with client
	{

		memset(buffer, 0, 1024);
		cout<<"Waiting Client Request...."<<endl;
        signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
        

		int len, n;
        n = recvfrom(server_socket, (char *)buffer,1024 , MSG_WAITALL, ( struct sockaddr *) &client_address, (socklen_t*)&client_address);               		
		// reading string sent by client
		
		if(!strcmp(buffer,"exit"))
		{
			cout<<"Client has quit from this session exit command"<<endl;                    
			//closing condition
            close(client_socket);
            close(server_socket);
            //goto lis;
            socketinit(portno);
			
		}
        if(!strcmp(buffer,"shut"))
		{
			cout<<"\n\nLOGOFF\n\n"<<endl;                    
			//closing condition
            close(client_socket);
            close(server_socket);
            exit(0);
			
		}
        if(n == 0 || n == -1)
		{
            cout<<"Client has quit from this session wrong return"<<endl;                    
			//closing condition
            close(client_socket);
            close(server_socket);
            //goto lis;
            socketinit(port);
        }
		
		//-----------Find Answer------------------
		string ck,input;
		input.clear();
		ck.clear();
		input = buffer;
		long long int op1 = 0,op2 = 0,answer = 0;
		char opt;
		ck = input[0];
		cout<<op1<<opt<<op2<<"  "<<ck<<"  "<<input;
		for(int i = 1 ; i < input.length() ; i++)                                //loop to iterate throw the given expression string
		{
			if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/')         //check for operator
			{
				op1 = atoi(ck.c_str());                                                 //converting string interger to integer number
				ck = input[i+1];
				opt = input[i];
				i = i + 2;
			}

			ck = ck + input[i];	
		}

		op2 = atoi(ck.c_str());
		ck = "0";
		cout<<"Client expression to solve.\n";
		cout<<op1<<" "<<opt<<" "<<op2<<endl;
		char msg[200];
		char *res;
		int flag = 0;
		switch(opt)                               			
		// to calculate values
		{
 			case '+': answer = op1 + op2;
			break;
 			case '-': answer = op1 - op2;
			break;
			case '*': answer = op1 * op2;
			break;
 			case '/': if(op2 == 0)
			 {
				 flag = 1;         							
				 //if op2 is 0 while dividing
				 break;
			 }
			 answer = op1 / op2;
			break;
		
		}
	
		//cout<<answer<<endl;
		stringstream strs;                                  
		// string stream for converting string to char*
		strs<<answer;
		string result = strs.str(),errora = "Cannot-divide-by-Zero";
		res = (char*)result.c_str();
		if(flag == 1)
		{
			res = (char*)errora.c_str();
		}
		cout<<res<<endl;

		//-----------Send Answer------------------

		sendto(server_socket, (const char *)res, strlen(res),MSG_CONFIRM, (const struct sockaddr *) &client_address,sizeof(client_address));             
		 // send expression reply to client
		cout<<"Message sent";
	}
	close(server_socket);            
}




int main(int argc, char ** argv)  		// taking command line inputs from user, file name and port number
{
	
	if(argc != 2)                            // checking if port number is provided or not
    {
        cerr <<"Usage: port"<< endl;
        exit(0);
    }
	
	//buffer to send and receve the data

	portno = atoi(argv[1]); 

    socketinit(portno);                          			 
	// converting cli port no to integer
    
    //close(client_socket);
	                      
	// shutting socket off.
	return 0;
}	