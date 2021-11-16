#include <iostream>
#include <windows.h>


int function_a(int x, int y) {
    return x + y;
}

int function_b() {
    std::cout << "hooked !" << std::endl;
    return -1;
}


int main() {
    
    std::cout << function_a << std::hex << function_b << std::endl;

    return 0;
}