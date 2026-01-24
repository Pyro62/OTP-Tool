
#include "encrypt.h"
#include <cstring>
void inputHandler(int,char const*[]);

// ISSUES:


int main(int argc, char const *argv[]){ 
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
            }else if(std::strcmp(argv[1],"-d")==0){
                enc.directoryEncrypt(argv[2],argv[3],argv[4]);
                
            } else {
                throw std::runtime_error("Invalid Arguments!");
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
