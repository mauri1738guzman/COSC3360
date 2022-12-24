#include<iostream>
#include <bits/stdc++.h>
#include<sstream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
 #include<string.h>
 #include<stdlib.h>
 #include<unistd.h>
 #include<pthread.h>
 #include<string>
 #include <netdb.h> 
 

using namespace std;

#define MAXSIZE 10

//-------------------------VARIABLES---------------------------

struct Shannon{
    char letter;
    double probabilities;
    double f_x;
    int argc;
    char** argv;
    string sendString;
    string final;
};


int arrayProbs[MAXSIZE];

//--------------------------DONE------------------------------




//-- This code was derived from geeksforgeeks
bool sortByVal(const pair<char, double> &a, 
               const pair<char, double> &b) 
{ 
    return (a.second > b.second); 
} 


//-- store the probabilites in to a map
map <char, double> getProbs(map<char, double> M, string str, int n){

    
    
//-- traverse through all chars

// Traverse string str check if
// current character is present
// or not
for (int i = 0; str[i]; i++)
{
    // If the current characters
    // is not found then insert
    // current characters with
    // frequency 1
    if (M.find(str[i]) == M.end())
    {
        M.insert(make_pair(str[i], 1));
    }

    // Else update the frequency
    else
    {
        M[str[i]]++;
    }
}

// Traverse the map to print the
// frequency
for(auto& it : M){
    it.second = it.second/n;
}
return M;
}


void getShannons(Shannon sh[], vector<pair<char, double>> v){

sh[0].letter = v[0].first;
sh[0].probabilities = v[0].second;

double sum = 0;
for(int i = 0; i < v.size(); i++){
    sh[i].letter = v[i].first;
sh[i].probabilities = v[i].second;
sh[i].f_x = sum+= sh[i].probabilities;

}
}

void *socket_funct(void *ptr){
//-- Define all the variables needed;
int sockfd, portno, n;
Shannon *c = (Shannon*) ptr;
struct sockaddr_in serv_addr;
struct hostent *server;


if(c->argc < 3){
    fprintf(stderr, "usage %s hostname port\n", c->argv[1]);
    exit(0);
}
portno = atoi(c->argv[2]);
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if(sockfd < 0){
    perror("ERROR OPENING SOCKET");
    exit(0);
}
server = gethostbyname(c->argv[1]);
if(server==NULL){
    fprintf(stderr, "Error, no such host\n");
    exit(0);
}
 bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr, 
     (char *)&serv_addr.sin_addr.s_addr,
     server->h_length);
serv_addr.sin_port = htons(portno);
if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
    perror("ERROR connecting");
    exit(0);
}

n = write(sockfd,c->sendString.c_str(),strlen(c->sendString.c_str()));
if (n < 0){ 
     perror("ERROR writing to socket");
     exit(0);
}
char buffer[256];
bzero(buffer, 256);

n = read(sockfd,buffer,255);
if (n < 0) {
     perror("ERROR reading from socket");
}

shutdown(sockfd,2);
c->final = buffer;    


close(sockfd);


return nullptr;


}


int main(int argc, char** argv){

string buffer;
getline(cin, buffer);
int n = buffer.length();
map <char, double> probOrder;
probOrder = getProbs(probOrder,buffer, n);


vector<pair<char, double>> sortedPairs;
map<char, double> :: iterator it1;
for(it1 = probOrder.begin(); it1!=probOrder.end(); it1++){
    sortedPairs.push_back(make_pair(it1->first, it1->second));
}
//-- sorte the pairs in order of letters or symbol
sort(sortedPairs.begin(), sortedPairs.end(), sortByVal);

//-- store the letters and the size of threads needed
int thread_count = sortedPairs.size();


//-- Declare Shannon variable
Shannon sh[thread_count];

getShannons(sh, sortedPairs);

pthread_t thread_sh[thread_count];
//-- Assign the argc and argv from main to struct
for(int i =0; i < thread_count; i++){
sh[i].argc = argc;
sh[i].argv = argv;
}


stringstream s;
cout << "SHANNON-FANO-ELIAS Codes:\n";
//-- Before sending to struct assing a string of all the variables needed
for(int i = 0; i < thread_count; i++){

if(i==0){
     s << sh[i].letter << " " << sh[i].probabilities << " " << sh[i].f_x << " 0";
}
else{
    s << sh[i].letter << " " << sh[i].probabilities << " " << sh[i].f_x << " " << sh[i-1].f_x;
}
    sh[i].sendString = s.str();

pthread_create(&thread_sh[i], nullptr, socket_funct,&sh[i]);
s.str("");

}

for(int i =0; i < thread_count; i++){
    pthread_join(thread_sh[i], nullptr);
}

for(int i =0; i < thread_count; i++){
    cout << sh[i].final << endl;
}

return 0;

}
