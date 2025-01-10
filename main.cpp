#include <iostream>
#include "file.h"
int main() {
    file f("User.csv");
    if(f.changePassword("pass3","newpas","user3")){
        std::cout<<"jes";
    }
    std::cout<<"jes";
    return 0;
}
