#include <iostream>
#include <string>
#include <cstring>
#include "Basic/Yaml.h"

// file type(i,n,s,f) name
int main(int argc, char **argv)
{
    if(argc <= 3){
        std::cout << "FAIL" << std::endl;
        return 0;
    }
    SLAM::Basic::YamlReader yaml(argv[1]);
    if(!yaml.IsLoaded()){
        std::cout << "FAIL" << std::endl;
        return 0;
    }
    if(0==strcmp(argv[2], "i")){
        int num = 0;
        if(!yaml.Read(argv[3], num)){
            std::cout << "FAIL" << std::endl;
            return 0;
        }
        std::cout << num << std::endl;
    }
    else if(0==strcmp(argv[2], "n")){
        unsigned int num = 0;
        if(!yaml.Read(argv[3], num)){
            std::cout << "FAIL" << std::endl;
            return 0;
        }
        std::cout << num << std::endl;
    }
    else if(0==strcmp(argv[2], "s")){
        std::string str;
        if(!yaml.Read(argv[3], str)){
            std::cout << "FAIL" << std::endl;
            return 0;
        }
        std::cout << str << std::endl;
    }
    else if(0==strcmp(argv[2], "f")){
        float num = 0.f;
        if(!yaml.Read(argv[3], num)){
            std::cout << "FAIL" << std::endl;
            return 0;
        }
        std::cout << num << std::endl;
    } 
    else{
        std::cout << "FAIL" << std::endl;
    }
    return 0;
}