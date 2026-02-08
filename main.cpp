
#include "encrypt.h"
#include <cstring>
#include <windows.h>
void inputHandler(int,char const*[]);

// ISSUES:


int main(int argc, char const *argv[]){ 
    inputHandler(argc,argv);
    
    return 0;
}

void inputHandler(int argc, char const * argv[]){
    Encrypt enc;
    try{
        auto hasFlag = [&](int pos, const char* flag){
            return pos < argc && std::strcmp(argv[pos], flag) == 0;
        };
        switch(argc) {
            case 4:
                if (hasFlag(1, "-f")) {
                    enc.createKeyFile(argv[2], argv[3]);
                } else {
                    enc.encrypt(argv[1], argv[2], argv[3]);
                }
                break;  
            case 5:
                if (hasFlag(1, "-f")) {
                    enc.encWithKey(argv[2], argv[3], argv[4]);
                } else if (hasFlag(1, "-d")) {
                    enc.directoryEncrypt(argv[2], argv[3], argv[4]);
                } else if (hasFlag(1, "-dr")) {
                    enc.dryRun(argv[0], argv[2], argv[3], argv[4]);
                } else {
                    throw std::runtime_error("Invalid Arguments!");
                }
                break;
            case 6:
                if (hasFlag(1, "-f") && hasFlag(2, "-d")) {
                    enc.encDirWithKey(argv[3], argv[4], argv[5]);
                } else {
                    throw std::runtime_error("Invalid Arguments!");
                }
                break;
                
            default:
                throw std::runtime_error("Invalid Arguments!");
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

}