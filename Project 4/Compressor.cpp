#include "provided.h"
#include <string>
#include <vector>
#include "HashTable.h"
#include <iostream>
using namespace std;

void Compressor::compress(const string& s, vector<unsigned short>& numbers)
{
    numbers.clear();
    
    unsigned int capacity = ((int)s.size()/2) + 512;
    if(capacity >= 16384) capacity = 16384;
    unsigned int numBuckets = capacity / 0.5;
    
    HashTable<string, unsigned short> H(numBuckets, capacity);
    
    // add 256 chars and their value
    for (unsigned short j = 0; j<= 255; j++) {
        string alphabetString = "";
        alphabetString = static_cast<char>(j);
        H.set(alphabetString, j, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    
    for(int c = 0; c < s.size(); c++){
        string expandedRun = runSoFar + s[c];
        unsigned short firstValue = 0;
        if(H.get(expandedRun, firstValue)){
            runSoFar = expandedRun;
        }else{
            unsigned short x = 0;
            H.get(runSoFar, x);
            numbers.push_back(x);
            H.touch(runSoFar);
            runSoFar = "";
            
            unsigned short cv = 0;

            string finalS = "";
            finalS = static_cast<char>(s[c]);
            H.get(finalS, cv);
            numbers.push_back(cv);
            
            if(!H.isFull()){
                H.set(expandedRun, nextFreeID);
                nextFreeID++;
            }else{
                string key = "";
                unsigned short value = 0;
                H.discard(key, value);
                H.set(expandedRun, value);
            }
        }
    }
    
    if(runSoFar.size() != 0){
        unsigned short x;
        H.get(runSoFar, x);
        numbers.push_back(x);
    }
    
    numbers.push_back(capacity);
    
}

bool Compressor::decompress(const vector<unsigned short>& numbers, string& s)
{
    if(numbers.empty()) return false;
    
    s = "";
    
    unsigned int capacity = numbers.back();
    unsigned int numBuckets = ((int)capacity / 0.5);
    
    HashTable<unsigned short,string> H(numBuckets, capacity);
    
    // add 256 numbers and their appropriate char
    for (unsigned short j = 0; j<= 255; j++) {
        string alphabetString;
        alphabetString = static_cast<char>(j);
        H.set(j, alphabetString, true);
    }
    
    unsigned short nextFreeID = 256;
    string runSoFar = "";
    string output = "";
    
    for(int i = 0; i < numbers.size()-1; i++){
        unsigned short us = numbers[i];
        if(us <= 255){
            string value = "";
            H.get(us, value);
            output = output + value;
    
            if(runSoFar.size() == 0){
                runSoFar = value;
            }else{
                string expandedRun = runSoFar + value;
            
                if(!H.isFull()){
                    H.set(nextFreeID, expandedRun);
                    nextFreeID++;
                }else{
                    unsigned short key = 0;
                    string someVal = "";
                    H.discard(key, someVal);
                    H.set(key, expandedRun);
                }
                runSoFar = "";
            }

 
        }else{
            string someVal = "";
            if(H.get(us, someVal) == false){
                cerr << "failure";
                return false;
            }else{
                H.touch(us);
                string S;
                H.get(us, S);
                output = output + S;
                runSoFar = S;
            }
        }
        
    }
    
    s = output;
    return true;
}




