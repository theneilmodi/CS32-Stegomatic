#include "provided.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
    if(hostIn.size() == 0) return false;
    hostOut = "";
    
    vector<string> lines;
    
    // seperating into lines by line break
    string contents = "";
    for(int i = 0; i<hostIn.size(); i++){
        char c = hostIn[i];
        
        if(c == '\n'){
            lines.push_back(contents);
            contents = "";
            continue;
        }
        
         contents = contents + c;
    }
    
    if(contents != "") lines.push_back(contents);
    
    
    //delete space, tab, returns at the end
    for(int j = 0; j<lines.size(); j++){
        string line = lines[j];
        for(int k = (int)line.size()-1; k>=0; k--){
            if(line[k] == '\t' || line[k] == ' ' || line[k] == '\r'){
                line.erase(line.begin()+k);
            }
            if(isalpha(line[k])) break;
        }
        lines[j] = line;
    }
    
    
    //compress and encode msg
    vector<unsigned short> numbers;
    Compressor::compress(msg, numbers);
    string encodedMessage = BinaryConverter::encode(numbers);
    
    //make encoded message vector
    int L = (int)encodedMessage.size();
    int N = (int)lines.size();
    
    int firstSet = L%N;
    int firstSize = (L/N)+1;
    int secondSet = N-firstSet;
    int secondSize = L/N;
    vector<string> encodedMessageVector;
    
    for(int f = 0; f<firstSet; f++){
        encodedMessageVector.push_back(encodedMessage.substr(0,firstSize));
        encodedMessage.erase(0,firstSize);
    }
    
    for(int f = 0; f<secondSet; f++){
        encodedMessageVector.push_back(encodedMessage.substr(0,secondSize));
        encodedMessage.erase(0,secondSize);
    }
    

    //add encoded message vector to lines vector
    for(int i = 0; i<lines.size(); i++){
        lines[i] = lines[i] + encodedMessageVector[i];
    }
    
    //setting hostout
    for(int i = 0; i<lines.size(); i++){
        hostOut = hostOut + lines[i] + "\n";
    }
    
    return true;
}


bool Steg::reveal(const string& hostIn, string& msg) 
{
    msg = "";
    
    vector<string> lines;
    
    // seperating into lines by line break
    string contents = "";
    for(int i = 0; i<hostIn.size(); i++){
        char c = hostIn[i];
        
        if(c == '\n'){
            lines.push_back(contents);
            contents = "";
            continue;
        }
        
        contents = contents + c;
    }

    if(contents != ""){
        lines.push_back(contents);
    }
    

    //extracts ends of vector
    string extraction;
    for(int j = 0; j<lines.size(); j++){
        string singleExtraction = "";
        string line = lines[j];
        for(int k = (int)line.size()-1; k>=0; k--){
            if(line[k] == '\t' || line[k] == ' '){ ///////////// CHANGE spaces-A    tab:'-'
                singleExtraction  = line[k] + singleExtraction ;
            }
            if(isalpha(line[k])) break;
        }
        extraction = extraction + singleExtraction;
    }
    
    // decode and decompress
    bool whatToReturn = true;
    
    vector<unsigned short> num;
    whatToReturn = BinaryConverter::decode(extraction, num);
    
    string myMessage = "";
    whatToReturn = Compressor::decompress(num, myMessage);
    msg = myMessage;

    return whatToReturn;
   
}

unsigned int computeHash(string s)
{
    // go through all chars in string and sum their ascii code
    int hash = 0;
    for(int i = 0; i<s.size(); i++){
        hash = hash + (int)s[i];
    }
    return hash;
}

unsigned int computeHash(unsigned short s)
{
    return s;
}



