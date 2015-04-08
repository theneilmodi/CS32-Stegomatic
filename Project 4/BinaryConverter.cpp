#include "provided.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

const size_t BITS_PER_UNSIGNED_SHORT = 16;

string convertNumberToBitString(unsigned short number);
bool convertBitStringToNumber(string bitString, unsigned short& number);

string BinaryConverter::encode(const vector<unsigned short>& numbers)
{
    string message = "";
    
    // go thorugh numbers vector and create encoded message
    for (int i = 0; i<numbers.size(); i++) {
        message = message + convertNumberToBitString(numbers[i]);
    }
    
    string encoded = "";
    
    // replace 0 and 1 with spaces and tabs
    for(int j = 0; j<message.size(); j++){
        if(message[j] == '0') encoded = encoded + ' '; //spaces
        else if(message[j] == '1') encoded = encoded + '\t'; //tabs
    }
    return encoded;  
}

bool BinaryConverter::decode(const string& bitString,
							 vector<unsigned short>& numbers)
{
    string mybitString = bitString;
    
    // if bitstring is not in multiples of 16
    if(mybitString.size() % 16 != 0) return false;
    
   // if bit string contains anything other than spaces and tabs
    for(int i = 0; i<mybitString.size(); i++){
        if(mybitString[i] != ' ' && mybitString[i] != '\t'){
            return false;
        }
    }
    
    //discarding stuff in vector numbers
    numbers.clear();
    
    while(mybitString.size() != 0){
        
        // get first 16 subset
        string subsetMessage = "";
        for(int j = 0; j<16; j++){
            subsetMessage = subsetMessage + mybitString[j];
        }
        
        // replace spaces and tabs with 0 and 1
        string bitMessage;
        for(int k = 0; k<subsetMessage.size(); k++){
            if(subsetMessage[k] == ' ') bitMessage = bitMessage + '0';
            else if(subsetMessage[k] == '\t') bitMessage = bitMessage + '1';
        }
        
        // convert to bit message to a number and add to vector
        unsigned short number = 0;
        convertBitStringToNumber(bitMessage, number);
        numbers.push_back(number);
        
        // remove first 16 chars bit string 
        mybitString.erase(0,16);
        
    }
    
    return true;
}

string convertNumberToBitString(unsigned short number)
{
	string result(BITS_PER_UNSIGNED_SHORT, '0');
	for (size_t k = BITS_PER_UNSIGNED_SHORT; number != 0; number /= 2)
	{
		k--;
		if (number % 2 == 1)
			result[k] = '1';
	}
	return result;
}

bool convertBitStringToNumber(string bitString, unsigned short& number)
{
	if (bitString.size() != BITS_PER_UNSIGNED_SHORT)
		return false;
	number = 0;
	for (size_t k = 0; k < bitString.size(); k++)
	{
		number *= 2;
		if (bitString[k] == '1')
			number++;
		else if (bitString[k] != '0')
			return false;
	}
	return true;
}
