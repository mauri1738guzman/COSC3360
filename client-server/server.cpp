/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   gcc server2.c -lsocket
*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<vector>
#include<sstream>
#include<iostream>

void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

using namespace std;

int main(int argc, char *argv[])
{
    
//-- This is our server code
//-- This code was derived from professor's site
 int sockfd, newsockfd, portno, clilen, pid;
 struct sockaddr_in serv_addr, cli_addr;

 if (argc < 2) {
     fprintf(stderr,"ERROR, no port provided\n");
     exit(1);
 }
 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if (sockfd < 0) 
    error("ERROR opening socket");
 bzero((char *) &serv_addr, sizeof(serv_addr));
 portno = atoi(argv[1]);
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = INADDR_ANY;
 serv_addr.sin_port = htons(portno);
 int yes = 1;
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(void*)&yes, sizeof(yes)) < 0) {
    fprintf(stderr,"ERROR,  SO_RESUEADDR\n");
     exit(1);
}
 if (bind(sockfd, (struct sockaddr *) &serv_addr,
          sizeof(serv_addr)) < 0) 
          error("ERROR on binding");
 listen(sockfd,5);
 clilen = sizeof(cli_addr);
 while (1) {
     newsockfd = accept(sockfd, 
           (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
     if (newsockfd < 0) 
         error("ERROR on accept");
     pid = fork();
     if (pid < 0)
         error("ERROR on fork");
     if (pid == 0)  {
         close(sockfd);
         dostuff(newsockfd);
         exit(0);
     }
     else close(newsockfd);
 } /* end of while */
 return 0; /* we never get here */
}

/******** DOSTUFF() *********************
This handles indefinite connections in the server
 *****************************************/
void dostuff (int sock)
{
int n;
char buffer[256];
  
bzero(buffer,256);
n = read(sock,buffer,255);
vector<string> out;
stringstream ss(buffer);
string s;
while(getline(ss,s, ' ')){
   out.push_back(s);
   
}

double L_i = log2(1/stod(out[1])) + 1;
int LValue = ceil(L_i);

double fbar = stod(out[1])*.5 + stod(out[3]);
string value;
double sum = fbar;
for(int i = 0; i < LValue;i++){
      sum *= 2;
        if(sum >= 1){
             value.push_back('1');
            sum-=1;
        }
        else{
            value.push_back('0');
        }
    }

stringstream finalChar;
finalChar << "Symbol " << out[0] << ", Code: " << value;

string correctOutput = finalChar.str();
if (n < 0) error("ERROR reading from socket");
n = write(sock,correctOutput.c_str(),25);
if (n < 0) error("ERROR writing to socket");
}