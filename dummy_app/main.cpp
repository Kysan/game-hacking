#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "mylib/lib.h"

int main()
{

    while (true)
    {
        Sleep(1000);
        std::cout << "Kysan(4): " << Kysan(4) << std::endl;
    }
    std::cout << "test" << std::endl;

    return 0;
}
