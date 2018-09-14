#include "allheader.h"
#include<poll.h>
#include <sys/ioctl.h>

#define MAX_CLIENTS 3

using namespace std;
string calc(char buffer[]);
int PORT;
void pollsort(struct pollfd fds[],int maxcli)
{
	for(int i=1;i<maxcli;i++)
	{
		for(int j=i+1;j<maxcli;j++)
		{
			if(fds[i].fd < fds[j].fd)
			{
				int temp = fds[i].fd;
				fds[i].fd = fds[j].fd;
				fds[j].fd = temp;

				short tempev = fds[i].events;
				fds[i].events = fds[j].events;
				fds[j].events = tempev;

				short temprev = fds[i].revents;
				fds[i].revents = fds[j].revents;
				fds[j].revents = temprev;
				
			}
		}
	}
}
int main(int argv , char **argc)
{
    if(argv < 2)
    {
        perror("Usage :Port");
        exit(EXIT_FAILURE);
    }
    PORT = atoi(argc[1]);
    int opt = 1;
    int master_socket , addrlen , new_socket , client_socket[30];
	int max_clients = 1 , activity, i , valread , fd;//,timeout =10000;
    int max_sd;
	int cliadd = 0;
    struct sockaddr_in address;
	struct pollfd fds[MAX_CLIENTS];

	memset(fds, 0, sizeof(fds));
      
    char buffer[1025];  
      
    //socket file descriptor
    fd_set readfds;
    char *message;
  
    //make all client flags zero
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //making main socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //making server to allow multiple connections 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //assigning address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;          //local network address
    address.sin_port = htons( PORT );
      
    //bind the socket to localhost  and port given in cli
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	cout<<"Listening to PORT:"<<PORT<<endl;
    
    if (listen(master_socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
   
    addrlen = sizeof(address);
	cout<<"Waiting for client\n";
	fds[0].fd = master_socket;
	fds[0].events = POLLIN;
	int totfd=0,flag =0;
	totfd++;
	max_clients = 1;
    while(1)
	{
		int nread;
		/*for(int  jj=1;jj<max_clients;jj++)
		{
			if(fds[jj].fd > 0 )
			{
				flag = 1;
				break;
			}
		}
		totfd = flag=0?1:totfd; */
		activity = poll(fds , MAX_CLIENTS,5000);
		if(activity == 0)
		{
			perror("TIMEOUT , No reading found on poll");
			for(int i=0 ; i<MAX_CLIENTS;i++)
			{
				cout<<fds[i].fd<<endl;
			}
			//exit(EXIT_FAILURE);
						
		}
		for(int i=0 ; i<max_clients ;i++)
		{
			//.if(fds[i].fd <= 0)
			if(fds[i].revents & POLLIN)
			{
				
                if(fds[i].fd == master_socket) 
				{
					if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            		{
            		    perror("accept");
            		    exit(EXIT_FAILURE);
            		}
            		
					/*if(totfd > 2)
					{
						for(int k=1;k<max_clients;k++)
						{
							if(fds[k].fd <= 0)
							{
								totfd = k;
								break;
							}
						}
					}*/
					/*for(int t=1;t<=max_clients;t++)
					{
						if(fds[t].fd <= 0)
						{
							totfd = t;
							break;
						}
					}*/
					if(fds[totfd].fd <= 0 && totfd<MAX_CLIENTS)
					{
						fds[totfd].fd = new_socket;
						fds[totfd].events = POLLIN;
					
						totfd++;
						cout<<"Adding Client of fd: "<<new_socket<<endl;
						max_clients++;
						/*if(totfd > MAX_CLIENTS)
						{
							pollsort(fds,max_clients);
						}*/
						cout<<"After Accept"<<endl;
						cout<<"New Connection from "<<inet_ntoa(address.sin_addr)<<":"<<ntohs(address.sin_port)<<endl;
					
					}
					
					
					//cliadd++;
				}
				else
				{
					ioctl(fds[i].fd, FIONREAD, &nread);

					if( nread == 0 )
                    {
                        close(fds[i].fd);
						
                        
					  
					   //totfd = i;
                        
                        
                         
                        fds[i].events = 0;
                        printf("Removing client on fd %d\n", fds[i].fd);
                        fds[i].fd = -1;
						cliadd--;
						for(int j=1;j<max_clients;j++)
						{
							if(fds[j].fd <= 0)
							{
								totfd = j;
								break;
							}

						}
						
                    }
                 
                	else 
					{
						
						int re = read(fds[i].fd , buffer , 1024);
						buffer[re] = '\0';
						cout<<buffer<<endl;
						
                    	cout<<"Client on fd: "<<fds[i].fd<<endl;
						string answer = calc(buffer);
						char * res = &answer[0u];
						send(fds[i].fd, res , strlen(res), 0);
						memset(buffer , 0 , sizeof(buffer));
                	}	
				}
				
			}
		}
	}	
}
string calc(char buffer[])
{	
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
	//sending back the answer to the particular client who has requested for server
	return res;
}