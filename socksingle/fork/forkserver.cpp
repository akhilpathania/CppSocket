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
using namespace std;
int sockfd, bind_stat;
void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;
    close(sockfd);
    close(bind_stat);

    exit(0);
}
int PORT;
int main(int argv , char **argc)
{
    if(argv < 2)
    {
        perror("Ussage :Port");
        exit(0);
    }
    PORT = atoi(argc[1]);


	
	struct sockaddr_in server_address;

	int newSocket;
	struct sockaddr_in client_address;

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
	if(listen(sockfd, 10) == 0)
    {
		cout<<"Listening....\n";
	}
    else
    {
		perror("Error in binding.\n");
	}

    //cout<<"Enter no of clients\n";
    //int cc;
    //cin>>cc;
    for(;;)
    {
		newSocket = accept(sockfd, (struct sockaddr*)&client_address, &addr_size);
		if(newSocket < 0)
        {
			exit(1);
		}
		cout<<"Connection accepted from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;

		if((childpid = fork()) == 0)
        {
			close(sockfd);
            cout<<"Child PID: "<<getpid()<<"  Parent ID: "<<getppid()<<endl;
			while(1)
            {
				char buffer[1024];

                int readval = read(newSocket , buffer , 1024);
                signal(SIGABRT, &sighandler);
	            signal(SIGTERM, &sighandler);
	            signal(SIGINT, &sighandler);
                if(readval <= 0)
                {
                    cout<<"Reading failed\n";
                    cout<<"Disconnected from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;
                    kill(getpid(),SIGTERM);
                    break;
                    exit(0);
                }
				if(strcmp(buffer, "exit") == 0 || strcmp(buffer, "shut") == 0)
                {
					if(strcmp(buffer, "shut") == 0)
                    {
                        cout<<"Logging Off\n";
                        close(sockfd);
                        close(bind_stat);
                        close(newSocket);
                        kill(getppid(),SIGQUIT);
                        //kill(getpid(),SIGKILL);
                        cout<<"Seesion ended\n";
                        exit(1);
                    }
                    cout<<"Disconnected from "<<inet_ntoa(client_address.sin_addr)<<": "<<ntohs(client_address.sin_port)<<endl;
                    kill(getpid(),SIGTERM);
                    break;
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

                    send(newSocket , res , sizeof(res), 0 );
					bzero(buffer, sizeof(buffer));
				}
			}
		}
        /*else 
        { 
            int status;
            // This is the parent process
        waitpid(childpid, &status, 0); // Wait for the child process to return.
        std::cout << "Process returned " << WEXITSTATUS(status) << ".\n";
        std::cout << "Press enter.\n";
        }*/

	}

	close(newSocket);
	return 0;
}