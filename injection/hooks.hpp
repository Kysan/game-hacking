

#ifndef A
#define A

#include "hooking.hpp"

int hKysan(int x)
{
    std::cout << "hook called" << std::endl;
    x += 1;
    int (*oKysan)(int) = (int (*)(int))backup.at("Kysan");

    return oKysan(x) + 1;
}

bool hfeatureAccessible(int flag)
{
    // bool (*ofeatureAccessible)(int) = (bool (*)(int))backup.at("featureAccessible");
    std::cout << "flag feature : " << flag << std::endl;
    // std::cout << "accessible : " << (ofeatureAccessible(flag) ? "oui" : "non") << std::endl;
    return true;
}

#endif