#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include<iostream>
#include<sstream>
#include<string>
using namespace std;
#define MAX_SIZE 50

int main()
{
    int listen_desc, conn_desc; // main listening descriptor and connected descriptor
    int maxfd, maxi; // max value descriptor and index in client array
    int i,j,k;  // loop variables
    fd_set tempset, savedset;  // descriptor set to be monitored
    int client[FD_SETSIZE], numready; // array of client descriptors
    struct sockaddr_in serv_addr, client_addr;
    char buff[MAX_SIZE];

    listen_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_desc < 0)
        printf("Failed creating socket\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(5656);
    setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
    if (bind(listen_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        printf("Failed to bind\n"); exit(0);

    listen(listen_desc, 5);

    maxfd = listen_desc; // Initialize the max descriptor with the first valid one we have
    maxi = -1; // index in the client connected descriptor array
    for (i=0; i<FD_SETSIZE; i++)
        client[i] = -1;  // this indicates the entry is available. It will be filled with a valid descriptor
    FD_ZERO(&savedset); // initialize the descriptor set to be monitored to empty
    FD_SET(listen_desc, &savedset); // add the current listening descriptor to the monitored set

    while(1) // main server loop
    {
        // assign all currently monitored descriptor set to a local variable. This is needed because select
        // will overwrite this set and we will lose track of what we originally wanted to monitor.
        tempset = savedset;
        numready = select(maxfd+1, &tempset, NULL, NULL, NULL); // pass max descriptor and wait indefinitely until data arrives

        //printf("Waiting\n");

        if(FD_ISSET(listen_desc, &tempset)) // new client connection
        {
            printf("new client connection\n");
            int size = sizeof(client_addr);
            conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr,(socklen_t *)&size);
            for (j=0; j<FD_SETSIZE; j++)
                if(client[j] < 0)
                {
                    client[j] = conn_desc; // save the descriptor
                    break;
                }

                FD_SET(conn_desc, &savedset); // add new descriptor to set of monitored ones
                if(conn_desc > maxfd)
                    maxfd = conn_desc; // max for select
                if(j > maxi)
                    maxi = j;   // max used index in client array
        }

        for(k=0; k<=maxi; k++) // check all clients if any received data
        {
            if(client[k] > 0)
            {
                if(FD_ISSET(client[k], &savedset))
                {
                    int num_bytes;
                    if( (num_bytes = read(client[k], buff, MAX_SIZE)) > 0)
                    {
                        buff[num_bytes] = '\0';
                        printf("Received:- %s", buff);

                        string ck,input;
		    input.clear();
		    ck.clear();
		    input = buff;
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

            send(client[k] , res , sizeof(res), 0 );
			bzero(buff, sizeof(buff));
                    }

                    if(num_bytes == 0)  // connection was closed by client
                    {
                        close(client[k]);
                        FD_CLR(client[k], &savedset);
                        client[k] = -1;
                    }

                    if(--numready <=0) // num of monitored descriptors returned by select call
                        break; 
                }
            }
        }
    } // End main listening loop

    close(listen_desc);
    return 0;
}