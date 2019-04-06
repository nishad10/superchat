#ifndef CLIENT_H
#define CLIENT_H

#include<iostream>
#include <string>

class message{

    public:
        std::string encode(std::string head, std::string body);

        std::string decode_head(std::string endc_str);

        std::string decode_body(std::string endc_str);

        void encrypt(std::string body);

        void decrypt(std::string body);

        std::string head;
        std::string body;
        int length;


};
#endif
