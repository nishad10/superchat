#ifndef CLIENTNEW_H
#define CLIENTNEW_H

#include<iostream>
#include <string>

class clientNew{

    public:
        std::string encode(std::string name1, int port1);
        
        std::string name;
        int port;
        


};
#endif