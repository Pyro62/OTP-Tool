
#include "encrypt.h"
#include <cstring>
void inputHandler(int,char const*[]);

// ISSUES:

// ./encrypt.exe -f plaintext.txt key.bin ciphertex2.txt DOES NOT CHECK KEY BEFORE CREATING DEST FILE. NO ERROR THROWN

// what done since last commit: reduced randomdevice calls, removed files generating on fail, refactor main


int main(int argc, char const *argv[]){ // TODO:  improve randomness, encrypt, decrypt directories. //curr alr works with large files because of chunking. // add flags handling and multiple modes
    inputHandler(argc,argv);
    
    return 0;
}

void inputHandler(int argc, char const * argv[]) // refactor to handle flags and stuff // enc.encrypt(argv[1],argv[2],argv[3]);
{
    Encrypt enc;
    try
    {
        if(argc==4){
            enc.encrypt(argv[1],argv[2],argv[3]);
        } else if(argc==5){
            if(std::strcmp(argv[1],"-f")==0){ // is 0 if equal
                enc.encWithKey(argv[2],argv[3],argv[4]);
            }else{
                throw std::runtime_error("argv not equal to flag");
            }
        } else if(argc==6){

        } else{
            throw std::runtime_error("Invalid Arguments!");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
