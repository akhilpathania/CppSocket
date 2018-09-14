#include<stdio.h>    
#include<iostream>
#include<stdlib.h>
//#include<sys/socket.h>                 // for socket fuctions
#include<netinet/in.h>                  // for address family
#include <unistd.h>
#include<string.h>
#include <sstream>						// string stream header		
#include<fstream>			
using namespace std;

int main(int argc, char ** argv)  		// taking command line inputs from user, file name and port number
{
	
	if(argc != 2)                            // checking if port number is provided or not
    {
        cerr <<"Usage: port"<< endl;
        exit(0);
    }
	char buffer[1024] = {0};
	//buffer to send and receve the data

	int portno = atoi(argv[1]);                           			 
	// conver   ting cli port no to integer



	int server_socket;
	
	//structure for getting server address
	struct sockaddr_in server_address;   							 
	//server information
	int addrlen = sizeof(server_address); 

	server_address.sin_family = AF_INET;           					
	// server type    here TCP

	server_address.sin_port = htons(portno);
	//assigning port number & also converting int port to xact type needed using htons

	server_address.sin_addr.s_addr = htonl(INADDR_ANY);      		
	// local server address

	server_socket = socket(AF_INET , SOCK_STREAM , 0);       // creating & initializing socket  			
	
	if(server_socket < 0)                             //failed in initializing socket
	{
        cerr<<"Error establishing the server socket"<<endl;    
		//unable to connect
        exit(0);
    }

	int bind_stat = bind(server_socket , (struct sockaddr *)&server_address , sizeof(server_address));     
	//bind port and address
	
	if(bind_stat < 0)                   // unable to bind the server address data conditioon.
	{
		cerr<<"Error Binding socket"<<endl;        	
			//unable to bind port
		exit(0);
	}
     
     
ss:
                                                                //got to statement for continuing server
	cout<<"Waiting for clients to connect..."<<endl;
	listen(server_socket , 5);      						
	// 5 requests can be fullfilled by server

	int client_socket;
	// waiting for some client inputs
   // while(1)
    //{
       client_socket = accept(server_socket ,(struct sockaddr *)&server_address,(socklen_t*)&addrlen);
        if(client_socket < 0)
	{
		cerr<<"Error accepting request from client!"<<endl;     			 
		// not accepting request
        exit(1);
	}
		cout<<"Waiting Client Request...."<<endl;
        
        close(server_socket); 
    
	//while(1)                                       								
	//after successfull connecting with client
	{
        again:
		memset(buffer, 0, 1024);
        
    //}

		int valread = read(client_socket , buffer ,1024); 
        cout<<buffer<<endl;
        string ans1(buffer);

		// reading string sent by client
		if(ans1 == "exit")
		{
			cout<<"Client has quit from this session"<<endl;                    
			//closing condition
            memset(buffer, 0, 1024);

			goto ss;
		}
        
        if(ans1 == "shutdown")
		{
			cout<<"Client has quit from this session"<<endl;                    
			//closing condition
            close(server_socket); 
			//break;
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

		send(client_socket , res , 21 , 0 );              
		 // send expression reply to client.
         char bu[1024] = {0};
         int valread1 = read(client_socket , bu ,1024); 
        cout<<bu<<endl;
        string ans11(bu);
        cout<<ans11;
        if(ans11  == "y")
        {
            goto again;
        }
        else
        {
            
            close(client_socket);
            goto ss;

            
        }
        
		
	}
	close(server_socket);                                  
	// shutting socket off.
	return 0;
	

}	