#include<stdio.h>
#include<pthread.h>
#include<string>
#include<stdlib.h>
#include<fstream>
#include<bits/stdc++.h>
#include<unistd.h>


using namespace std;


/*
 * Created By: Mauricio Perez
 * Date: 09/12/2022
 * Professor: Rincon
 * Class: COSC 3360
 */
 
 

#define MAX_SIZE 100

//-------------- declare variables-------------------
struct shannon_variables{
    string letter;        //-- letter will be assigned to probability
    double prob;        //-- values will be given for probability
    double f_x;         //-- f(x) += f(x) + p(x)
    double f_xPrime;    //-- equation is p(x).5 + f(x-1)
    int L_i;         //-- L(i) value with number of shannon code characters
    string code;        //-- shannon code value
    

};


string letters = "";
string probabilities = "";

string arrayLetters[MAX_SIZE];
int num_probabilities = 0;

shannon_variables var[MAX_SIZE];

//-- these were just added


 




//-------------------DONE--------------------------

//-- access file.tx or argv[1]
void getInputs( string letters, string probabilities){





//-- after setting the file ifstream infile, we will 
//-- input the first row into the letters string 
//-- and the second row into the probabilities string








//cout << "Here is the letters:\n" << letters << endl;
//cout << "Here is the probabilities:\n" << probabilities << endl;




string tempStr = "";
    for (auto x : letters)
    {
        if (x == ' ')
        {
            var[num_probabilities].letter = tempStr;
            tempStr = "";
            num_probabilities++;
        }
        else {
            tempStr = tempStr + x;
        }
    }
    
    var[num_probabilities].letter = tempStr;
    num_probabilities++;
    //cout << "----------------------------------------------------\n";
    //cout << "There is " << num_probabilities << " numbers of probabilities\n";

    //----------------------------------------------
//---------------------------------------------
//-- Here we will do the same but the probabilities will be saved
int count = 0;
 
    string word = "";
    for (auto x : probabilities)
    {
        if (x == ' ')
        {
            var[count].prob = atof(word.c_str());
            word = "";
            count++;
        }
        else {
            word = word + x;
        }
    }
    
    var[count].prob = atof(word.c_str());
    count++;
// cout << "---------------------------------\n";
// cout << "---------------------------------\n";
// cout << "Here is the array of lettters\n";
// //-- here we will convert to a double array instead of string
// for(int i = 0; i < count; i++){
    
//     cout << i  << " ::: " << var[i].letter << endl;
// }









// cout << "---------------------------------\n";
// cout << "---------------------------------\n";
// cout << "Here is the array of probabilities\n";
// //-- here we will convert to a double array instead of string
// for(int i = 0; i < count; i++){
    
//     cout << i  << " ::: " << var[i].prob << endl;
// }





}








void *getCode(void *ptr){

    //-- declare variable inside 
     shannon_variables *arg = (shannon_variables*) ptr;
    
    //-- here we will acess a different then the other letters
    if(arg->letter == "A" || arg->letter == "a" || arg->letter == "1"){
        arg->f_x = arg->prob;
        arg->f_xPrime = arg->prob*.5;
     
       
    }
    //-----------------------------------------------------------------
    
    
        
        //-- This needs to fixed 
        arg->f_x = arg->prob + arg[-1].f_x;       //-- HELP HERE
         arg->f_xPrime = arg[-1].f_x + ((arg->prob)*.5);
        
    
    //-----------------------------------------------------------------
       
        //-- here we have calculated the L(i)
     double value = log2(1/arg->prob) + 1;
            arg->L_i = ceil(value);


        //-- here starts the code into binary conversion
        double sum = arg->f_xPrime;
        for(int i = 0; i < arg->L_i; i++){
            sum *= 2;
            
            if(sum >= 1){
                 arg->code.push_back('1');
                sum-=1;
            }
            else{
                arg->code.push_back('0');
                
            }
        }
        
        //-- this will print in order---------
        
        
        
        printf("Symbol ");
        printf(arg->letter.c_str());
        printf(", Code: ");
        printf(arg->code.c_str());
        printf("\n");
   
      
        return nullptr;
    
}




int main(int argc, char** argv){

     
     
        getline(cin, letters);
        getline(cin, probabilities);
     
   
    
    //-- will run a shannon code using pthreads 
    //------------------------------------------------------
    //-- pthread_create(&pthread,NULL,getletters, &var[0]);
    //------------------------------------------------------
    getInputs(letters, probabilities);

    pthread_t pthread[num_probabilities];

    
    //-- Here is the printing out of the code
    cout << "SHANNON-FANO-ELIAS Codes:  " << endl << endl;
    //-- here is where they will be created
    for(int i = 0; i < num_probabilities; i++){
       
        
      pthread_create(&pthread[i], nullptr, getCode, &var[i]);
      sleep(1);
         
         
  
}
   
    //-- pthread_join(pthread, nullptr);
    
  
    
     for(int i = 0; i < num_probabilities; i++){
          //-- Here we will print out current struct
    
   
      pthread_join(pthread[i], nullptr);
          
       
         
     }
    
    
   
 
     


   



    return 0;
}


