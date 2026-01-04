
#include "encrypt.h"

//todo, add exxcpt if dest file is shit

int main(int argc, char const *argv[]){ // todo, generate key and feed it
    try
    {
        Encrypt enc;
        if(argc==4){ 
            enc.encrypt(argv[1],argv[2],argv[3]); // if key is provided
        } else if(argc == 3){
            enc.encrypt(argv[1], argv[2]);
        } else {
            throw std::runtime_error("Invalid Arguments!");
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    



    return 0;
}