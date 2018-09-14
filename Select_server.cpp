#include "allheader.h"
using namespace std;
 
int PORT;
int main(int argv , char **argc)
{
    if(argv < 2)
    {
        perror("Ussage :Port");
        exit(EXIT_FAILURE);
    }
    PORT = atoi(argc[1]);
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30];
	int max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;
      
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
    address.sin_addr.s_addr = inet_addr("10.73.73.150");          //local network address
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
     
    while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add main socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add client sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //client socket file descriptor
            sd = client_socket[i];
             
            //9if valid add to set
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //finding max fd
            if(sd > max_sd)
                max_sd = sd;
        }
        struct timeval timeout;
        timeout.tv_sec  = 5;
        timeout.tv_usec = 0;

        //wait for an activity on any one of the sockets 
        cout<<"Before select"<<endl;
      
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            perror("select error:");
        }
        
        //to check some happening in fd_set through socket
        cout<<"after select and before ISSET"<<endl;
        if (FD_ISSET(master_socket, &readfds))      //this will check the recent socket which triggered select in readfds
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            cout<<"After Accept"<<endl;
			cout<<"New Connection from "<<inet_ntoa(address.sin_addr)<<":"<<ntohs(address.sin_port)<<endl;

            for (i = 0; i < max_clients; i++) 
            {
                //assigning new client to previous client place who has quited session
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
					cout<<"Adding This client in "<<i<<" position"<<endl;
                    break;
                }
            }
        }
          
        //loop for fulfilling any clients request
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds)) 
            {
                cout<<":Before read after FDISSET"<<endl;
                //checking and reading from client
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //if any client get disconnected
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    cout<<"Client Disconnected "<<inet_ntoa(address.sin_addr)<<":"<<ntohs(address.sin_port)<<endl;
					  
                    //Close the socket and make client place 0 for reusing
                    close( sd );
                    client_socket[i] = 0;
                }
				
                  
                //for sending answer back to client
                else
                {
                    cout<<"Kaam Chalu"<<endl;
                     buffer[valread] = '\0';
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
                    send(sd , res , strlen(res) , 0 );
                }
            }
        }
    }
      
    return 0;
} 