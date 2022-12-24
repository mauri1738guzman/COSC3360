// Write your program here
#include<iostream>
#include<bits/stdc++.h>
#include <pthread.h>
#include<stdio.h>
#include<unistd.h>


struct Shannon{
    char name;
    double p_x;
    double f_x;
    double f_bar;
    int L_i;
    int count;
    std::string code;
 
};

//-- Function to get occurences in map
std::map<char,double> printFrequency(std::string str)
{
    // Define an unordered_map
       std::map<char, double> M;
 
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
            M.insert(std::make_pair(str[i], 1));
        }
 
        // Else update the frequency
        else
        {
            M[str[i]]++;
        }
    }
 for (auto& it : M) {
         it.second = it.second/str.size();
    }
   
    
    return M;
}
 

void *findShannon(void *ptr){
    Shannon *sh_ptr = ( Shannon*) ptr;
    
    //-- declare static variables here
    static int turn;
    static pthread_mutex_t bsem;
    static pthread_cond_t waitTurn;
    
    if(sh_ptr->count == 0){
        pthread_mutex_init(&bsem, NULL);
        pthread_cond_init(&waitTurn, NULL);
        turn = 0;
    }    
    
    
       pthread_mutex_lock(&bsem);
    while(sh_ptr->count != turn){
       pthread_cond_wait(&waitTurn, &bsem);
    }
    
    pthread_mutex_unlock(&bsem);
        
    

    pthread_mutex_lock(&bsem);
    //-- This is the start of Shannon
    
    sh_ptr->f_bar = sh_ptr[-1].f_x + (sh_ptr->p_x*.5);
    
    double value = log2(1/sh_ptr->p_x) + 1;
    sh_ptr->L_i = ceil(value);
    
    
    double sum = sh_ptr->f_bar;
    for(int i = 0; i < sh_ptr->L_i; i++){
      sum*= 2;
      
      if(sum >= 1){
          sh_ptr->code.push_back('1');
          sum -=1;
          
      }
      else{
          sh_ptr->code.push_back('0');
      }
    }
    
    
    std::cout << "Symbol " << sh_ptr->name << ", Code: " << sh_ptr->code << std::endl;
    
    turn++;
    pthread_cond_broadcast(&waitTurn);
    pthread_mutex_unlock(&bsem);
    
   
    
        
    return nullptr;
    
}

int main(){
    
  
  
    std::string input;
    std::cin >> input;
    // std::cout << "This is the input\n";
    // std::cout << input << std::endl;
    std::map<char, double> freq = printFrequency(input);
     // Traverse the map to print the
    // frequency
    //std::cout << "This is the main: " << std::endl;
    // for (auto& it : freq) {
    //     std::cout << it.first << ' ' << it.second << '\n';
    // }
    
    // std::cout << "------------------------\n";
    // std::cout << "number of threads \n";
    //-- This is the n POSIX threads
    int n = freq.size();
    //std::cout << n << std::endl;
    
    //-- Initialize array of threads and semaphores
    pthread_t pid[n];
    
    struct Shannon sh[n];
    //-- Save to structure 
    int count = 0;
    double sum = 0;
    //-- insert into struct
     for (auto& it : freq) {
    
        sh[count].name = it.first;
        sh[count].p_x = it.second;
        sum += it.second;
        sh[count].f_x = sum;
        count++;
     }
     
    
    // std::cout << "Here is the structure\n";
    // for(int i = 0; i < n; i++){
    // std::cout << sh[i].name << " ::: " << sh[i].p_x << " sum: " << 
    //  sh[i].f_x << std::endl;
    // }
    
    // std::cout << std::endl;
    // std::cout << "------------------------\n";
    //-- start initializing 
  
   
     std::cout << "SHANNON-FANO-ELIAS Codes:" << std::endl;
     for(int i = 0; i < n; i++){
       sh[i].count = i;
        
    //-- std::cout << "The i: " << sh[i].count << " the turn: " << sc.turn << std::endl;
    
    if(pthread_create(&pid[i], nullptr,findShannon, &sh[i])){
        std::cerr << "Failed to create thread";
        return 1;
    }
    }
    
    
    
    for(int i = 0; i < n;i++){
        pthread_join(pid[i], nullptr);
    }
        


    
    
    
    
    
    
    return 0;
    
}