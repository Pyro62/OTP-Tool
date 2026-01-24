#include "encrypt.h"

#include <random>
#include <filesystem>



void Encrypt::encrypt(const std::string& srcFilename,const std::string& keyFilename,const std::string& dstFilename)
{
    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream key{keyFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");

    
    if(!key) throw std::runtime_error("Could not open key");
    if(!src) throw std::runtime_error("Could not open source");

    std::ofstream dst{dstFilename, std::ios::binary};
    if(!dst) throw std::runtime_error("Could not open destination file"); // dst as final check, so on fail no creation
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



void Encrypt::encWithKey(const std::string &srcFilename, const std::string& keyName, const std::string &dstFilename) // house made keys
{
    std::ifstream src{srcFilename, std::ios::binary};
    std::ifstream check{dstFilename};
    if(check) throw std::runtime_error("Destination file already exists! Try again");
    
    if(!src) throw std::runtime_error("Could not open source");
    if(std::filesystem::exists(keyName)) throw std::runtime_error("Key file name already exists, please wait a second"); //if exists kick out
    std::ofstream dst{dstFilename, std::ios::binary};
    if(!dst) throw std::runtime_error("Could not open destination file");// dst as final check, so on fail no creation
    padWithKey(src, keyName ,dst);

}

void Encrypt::directoryEncrypt(const std::string &srcDir, const std::string &keyName, const std::string &dstDir)
{
    // todo: iterate through everything and encrypt with original encrypt function, then return a copy with the same directory // 1/23 done
    // IDEA: ENCRYPT AND RECREATE AS WE ITERATE, SO WE OPEN A FOLDER===> SIGNALS TO CREATE ONE // done
    std::filesystem::path rootDirectory{srcDir};
    if(!std::filesystem::exists(rootDirectory) || !std::filesystem::is_directory(rootDirectory)){
    throw std::runtime_error("Source directory does not exist or is not a directory");
    }
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDirectory)) {
        if(entry.is_directory()){
            std::filesystem::create_directories(std::filesystem::path(dstDir) / std::filesystem::relative(entry.path(), rootDirectory)); // create mirror dir in dst
        } else if(entry.is_regular_file()){
            // encrypt file and place in new dir
            std::filesystem::path srcPath{entry.path()};
            std::filesystem::path dstPath = std::filesystem::path(dstDir) / std::filesystem::relative(srcPath, rootDirectory);
            std::filesystem::path encFileName{srcPath.stem().string() + "_encrypted" + srcPath.extension().string()};
            std::filesystem::path finalDstPath{dstPath.parent_path() / encFileName};
            std::filesystem::create_directories(finalDstPath.parent_path());
            encrypt(srcPath.string(),keyName,finalDstPath.string());
        } else{
            std::cout << "Skipping non-regular file: " << entry.path() << "\n";
        }
    }
}

void Encrypt::padWithKey(std::ifstream &src, const std::string &keyName, std::ofstream &dst)
{
    std::random_device rd;
    std::ofstream key{keyName, std::ios::binary};
    if(!key) throw std::runtime_error("Could not generate key"); // check if can generate key/if can open
    char keyBuffer[BUFFER_SIZE];
    char srcBuffer[BUFFER_SIZE];
    size_t srcByteQty;
    while(true){
        src.read(srcBuffer,BUFFER_SIZE);
        srcByteQty = src.gcount();
        if(srcByteQty==0) break;
        genKey(keyBuffer,srcByteQty, rd); //key.read(keyBuffer,srcByteQty); // read srcbyteqty instead of buffer size so they usually match, also pulls key
        for (size_t i{0}; i < srcByteQty; i++)
        {
            srcBuffer[i] = srcBuffer[i]^keyBuffer[i];
        }
        dst.write(srcBuffer, srcByteQty);
        key.write(keyBuffer, srcByteQty);
    }
}

void Encrypt::genKey(char buffer[BUFFER_SIZE], size_t byteQty, std::random_device& rd)// modifies buffer with random stuff, returns it
{
     // move to pad, so one rd per operation // todo, remove multiple rd() calls, do research and stuff to seed a good rng stuff
    for (size_t i = 0; i < byteQty; i++)
    {
        
        buffer[i] = static_cast<char>(rd());
    }
    
}
