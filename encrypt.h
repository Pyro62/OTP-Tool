#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <fstream>
#include <stdexcept> 
#include <cstddef>
#include <iostream>

#define BUFFER_SIZE 1024

class Encrypt{
    public:
        void encrypt(const std::string&, const std::string&,const std::string&);
        void encrypt(const std::string&, const std::string&); // two arg, file and dest, key is generated
    private:
        void pad(std::ifstream&,std::ifstream&,std::ofstream&);
        void pad(std::ifstream&,std::ofstream&); // second pad for keygen
        void genKey(char[BUFFER_SIZE], size_t);


};




#endif // encrypt H