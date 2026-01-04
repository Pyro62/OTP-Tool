#include "encrypt.h"

#include <random>



void Encrypt::encrypt(const std::string& srcFilename,const std::string& keyFilename,const std::string& dstFilename)
{
    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream key{keyFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");
    check.close();

    std::ofstream dst{dstFilename, std::ios::binary};
    if(!key) throw std::runtime_error("Could not open key");
    if(!src) throw std::runtime_error("Could not open source");
    if(!dst) throw std::runtime_error("Could not open destination file");
    pad(src,key,dst);

}



void Encrypt::pad(std::ifstream& src, std::ifstream& key, std::ofstream& dst)
{
    char keyBuffer[BUFFER_SIZE];
    char srcBuffer[BUFFER_SIZE];
    size_t keyByteQty;
    size_t srcByteQty;
    while(true){
        src.read(srcBuffer,BUFFER_SIZE);
        srcByteQty = src.gcount();
        key.read(keyBuffer,srcByteQty); // read srcbyteqty instead of buffer size so they usually match
        keyByteQty = key.gcount();
        if(srcByteQty > keyByteQty) throw std::runtime_error("Invalid key, too short for file");
        if(srcByteQty==0) break;
        for (size_t i{0}; i < srcByteQty; i++)
        {
            srcBuffer[i] = srcBuffer[i]^keyBuffer[i];
        }
        dst.write(srcBuffer, srcByteQty);
    }
}



void Encrypt::encrypt(const std::string &srcFilename, const std::string &dstFilename) // house made keys
{
    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");
    check.close();
    std::ofstream dst{dstFilename, std::ios::binary};
    if(!src) throw std::runtime_error("Could not open source");
    if(!dst) throw std::runtime_error("Could not open destination file");
    pad(src,dst);

}

void Encrypt::pad(std::ifstream &src, std::ofstream &dst)
{
    
    std::string keyFilename{"key_" + std::to_string(time(nullptr)) + ".bin"}; // generate unique filename
    std::ofstream key{keyFilename, std::ios::binary | std::ios::app};
    if(!key) throw std::runtime_error("Could not generate key"); // check if can generate key/if can open
    if(key.tellp() != 0) throw std::runtime_error("Key file name already exists, please wait a second"); // when opened, is it empty? if not then throw
    char keyBuffer[BUFFER_SIZE];
    char srcBuffer[BUFFER_SIZE];
    size_t srcByteQty;
    while(true){
        src.read(srcBuffer,BUFFER_SIZE);
        srcByteQty = src.gcount();
        if(srcByteQty==0) break;
        genKey(keyBuffer,srcByteQty); //key.read(keyBuffer,srcByteQty); // read srcbyteqty instead of buffer size so they usually match, also pulls key
        for (size_t i{0}; i < srcByteQty; i++)
        {
            srcBuffer[i] = srcBuffer[i]^keyBuffer[i];
        }
        dst.write(srcBuffer, srcByteQty);
        key.write(keyBuffer, srcByteQty);
    }
}

void Encrypt::genKey(char buffer[BUFFER_SIZE], size_t byteQty) // modifies buffer with random stuff, returns it
{
    std::random_device rd;
    for (size_t i = 0; i < byteQty; i++)
    {
        
        buffer[i] = static_cast<char>(rd());
    }
    
}
