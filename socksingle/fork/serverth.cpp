#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<sstream>
#include<string>
#include<signal.h>
#include<sys/wait.h>
#include<pthread.h>
#include<sys/time.h>
using namespace std;
int sockfd, bind_stat;
int newSocket[3];
struct sockaddr_in server_address;
struct sockaddr_in client_address;
void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;
    //pthread_cancel(pthread_self());
    //pthread_exit(NULL);
            
    close(sockfd);
    close(bind_stat);
    shutdown(sockfd,2);
    exit(0);
}
int PORT;
void *calmeth(void *dummyPT);
int main(int argv , char **argc)
{
    if(argv < 2)
    {
        perror("Ussage :Port");
        exit(0);
    }
    fd_set readfds;
    PORT = atoi(argc[1]);
    pthread_t threadA[3];
    socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
    {
		perror("Error in connection.\n");
		exit(1);
	}
	cout<<"Server Socket is created.\n";

	memset(&server_address, '\0', sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
   
	bind_stat = bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
	if(bind_stat < 0)
    {
		perror("Error in binding.\n");
		exit(1);
	}
	cout<<"Bind to port: "<<PORT<<endl;
	if(listen(sockfd, 5) == 0)
    {
		cout<<"Listening....\n";
	}
    else
    {
		perror("Error in binding.\n");
	}

    int threadc = 0;
    
    while(1)
    {

        newSocket[threadc] = accept(sockfd, (struct sockaddr*)&client_address, &addr_size);
		if(newSocket[threadc] < 0)
        {
			exit(1);
		}
		cout<<"Connection accepted from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;

        pthread_create(&threadA[threadc], NULL , calmeth ,(void *)&newSocket[threadc]);

        threadc++;
 
	}
    
	close(sockfd);
	return 0;
}

void *calmeth(void *newSock)
{
    cout << "Thread No: " << pthread_self() << endl;
    while(1)
    {
		char buffer[1024];
        int conId = *((int *)newSock);
        int readval = read(conId , buffer , 1024);
        signal(SIGABRT, &sighandler);
	    signal(SIGTERM, &sighandler);
	    signal(SIGINT, &sighandler);
        if(readval <= 0)
        {
            cout<<"Reading failed\n";
            cout<<"Disconnected from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;
            close(conId);
            //pthread_cancel(pthread_self());
            pthread_exit(NULL);
            break;
        }
		if(strcmp(buffer, "exit") == 0 || strcmp(buffer, "shut") == 0)
        {
			if(strcmp(buffer, "shut") == 0)
            {
                cout<<"Logging Off\n";
                close(sockfd);
                close(bind_stat);
                close(conId);
                exit(0);
                //pthread_cancel(pthread_self());
                pthread_exit(NULL);
            
                cout<<"Seesion ended\n";
                
            }
            cout<<"Disconnected from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;
            close(conId);
            //spthread_cancel(pthread_self());
            pthread_exit(NULL);
            
            
		}
        else
        {
			        //find answere-----
            string ck,input;
		    input.clear();
		    ck.clear();
		    input = buffer;
		    long long int op1 = 0,op2 = 0,answer = 0;
		    char opt;
		    ck = input[0];
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

            send(conId , res , sizeof(res), 0 );
			bzero(buffer, sizeof(buffer));
    
		}
	}
close(sockfd);

}