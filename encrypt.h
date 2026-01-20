#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <fstream>
#include <stdexcept> 
#include <cstddef>
#include <iostream>
#include <random>

#define BUFFER_SIZE 1024

class Encrypt{
    public:
        void encrypt(const std::string&, const std::string&,const std::string&);
        void encWithKey(const std::string&, const std::string&, const std::string&); // two arg, file and dest, key is generated
    private:
        void pad(std::ifstream&,std::ifstream&,std::ofstream&);
        void padWithKey(std::ifstream&, const std::string& ,std::ofstream&); // second pad for keygen
        void genKey(char[BUFFER_SIZE], size_t, std::random_device&);


};




#endif // encrypt H