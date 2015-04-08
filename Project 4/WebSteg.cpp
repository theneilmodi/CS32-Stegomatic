#include "provided.h"
#include <string>
#include "http.h"
#include <iostream>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{

    string page;
    if(HTTP().get(url,page)){ //get page
        bool whatToReturn = Steg::hide(page, msg, host);
        return whatToReturn;
    }else{
        cerr << "Error fetching content from URL " << url << endl;
        return false;
    }
    
    return false;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string page;
    if(HTTP().get(url,page)){ //get page
        bool whatToReturn = Steg::reveal(page, msg);
        return whatToReturn;
    }else{
        cerr << "Error fetching content from URL " << url << endl;
        return false;
    }
    
    return false;
}



