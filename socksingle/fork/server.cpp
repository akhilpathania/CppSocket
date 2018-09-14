#include<sys/socket.h>
#include<sys/types.h>
#include<iostream>
#include<stdlib.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<sstream>
#include<signal.h>
using namespace std;

int main(int argv, char **argc)
{
    if(argv < 2)
    {
        perror("Usage : PORT");
        exit(EXIT_FAILURE);
    }
    int portno = atoi(argc[1]);

    struct sockaddr_in server_address;
    int addrlen = sizeof(server_address);
    
    //Server address assign
    server_address.sin_family = AF_INET;
    server_address.sin_port  = htons(portno);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int sockfd = socket(AF_INET, SOCK_STREAM , 0);
    if(sockfd < 0)
    {
        perror("Socket creation Failed\n");
        close(sockfd);
        exit(0);
    }

    int bind_status = bind(sockfd , (struct sockaddr*)&server_address, sizeof(server_address));
    if(bind_status <  0)
    {
        perror("Binding Failed");
        close(sockfd);
        exit(0);
    }

    cout<<"Waiting for Clients\n\n";
    pid_t pid;
    cout<<"Enter No of Clients to handle.\n>";
    int cc,counter=0;;
    cin>>cc;
    for(int i=0;i<cc;i++) 
    {
        
    
    if(fork() == 0)
    { 
        counter++;
        listen(sockfd, 5);
        int client_socket = accept(sockfd, (struct sockaddr*)&server_address , (socklen_t *)&addrlen);
        if(client_socket < 0)
        {
           perror("Unable to accept from client\n");
           close(sockfd);
           exit(EXIT_FAILURE);
        }

        cout<<"Connected with Client SuccessFully\n";
        char buffer[1024];
        int readval = read(client_socket , buffer , 1024);
        if(readval < 0)
        {
            cout<<"Reading failed\n";
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    

        //find answere-----
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

        send(client_socket , res , sizeof(res), 0 );
        close(sockfd);
        close(client_socket);
        cout<<"\nChild Process Completed having id= "<<getpid()<<" and Parent id= "<<getppid();
        exit(0);
        cout<<"LOGOUT\n";
        
        
    }
    
    }
    
    return 0;
}