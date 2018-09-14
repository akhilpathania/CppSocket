// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 #include<iostream>
 #include<sstream>
 #include<signal.h>
 using namespace std;

#define MAXLINE 1024
 
// Driver code
void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;
    exit(0);
}
int main(int argv , char ** argc) {


    if(argv <2)
    {
        perror("Ussage : PORT");
        exit(0)
;    }
    int PORT = atoi(argc[1]);
    yy:
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
     
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
     
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
     
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
     
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
     ss:


     memset(buffer, 0, 1024);
	cout<<"Waiting Client Request...."<<endl;
    signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

    int len, n;
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL,(struct sockaddr *) &cliaddr,(socklen_t*)&len);
    buffer[n] = '\0';
    cout<<endl<<n<<endl;
    string anss(buffer);
    if(anss == "shut")
    {
        cout<<"Client Quited\n";
        close(sockfd);
        exit(0);
    }
    if(anss == "exit")
    {
        goto ss;
    }
    //if(n == 0 )
		//{
     //       cout<<"Client has quit from this session from exit commands"<<endl;                    
			//closing condition
      //      close(sockfd);
       // }
    //---------find and--

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
		cout<<"ANswer is: "<<res<<endl;




    printf("Client : %s\n", buffer);
    hello = res;
    sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *)&cliaddr,len);
    printf("Message send\n");
    sleep(1); 
     goto ss;
    return 0;
}