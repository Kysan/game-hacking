#include "lib.h"
#include <iostream>



int Kysan(int x)  {
    std::cout << "dll function called !" << std::endl;
    return x + 1;
}