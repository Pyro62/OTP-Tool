#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <fstream>
#include <stdexcept> 
#include <cstddef>
#include <iostream>
#include <random>
#include <filesystem>
#include <vector>
#define BUFFER_SIZE 1024

class Encrypt{
    public:
        void encrypt(const std::string&, const std::string&,const std::string&);
        void encWithKey(const std::string&, const std::string&, const std::string&); // two arg, file and dest, key is generated
        void directoryEncrypt(const std::string&, const std::string&, const std::string&);
        void encDirWithKey(const std::string&, const std::string&, const std::string&);
        void dryRun(const std::string&, const std::string&, const std::string&, const std::string&);
        void createKeyFile(const std::string &,const std::string &); // src, key, respectively
    private:
        void pad(std::ifstream&,std::ifstream&,std::ofstream&);
        uintmax_t keyOffset{0};
        void generateKey(uintmax_t, const std::string &);
        

};




#endif // encrypt H