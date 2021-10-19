#include <iostream>
#include <windows.h>


int main() {
    HINSTANCE dllHandle = LoadLibrary("./mylib.dll");
    if (dllHandle == NULL) {
        std::cout << "echec du chargement de la dll" << std::endl;
        return -1;
    }

    std::cout << "le chargement de la dll est un succes" << std::endl;
    
    // * on cherche l'adresse de la fonction dans l'instance de la DLL en mémoire
    int (*ptrFonction)(int) = (int (*)(int)) GetProcAddress(dllHandle, "kysan");

    if(ptrFonction == NULL) {
        std::cout << "la fonction n'a pas été trouvé dans la dll" << std::endl;
        return -1;
    }


    // * je test la fonction
    std::cout << "Test de la fonction: " << std::endl;
    int a = 5;
    std::cout << "a: "<< a << std::endl;
    std::cout << "appel de la fonction avec a en parametre " << std::endl;
    a = (*ptrFonction)(a);
    std::cout << a << std::endl;



    if(!FreeLibrary(dllHandle)) {
        std::cout << "echec de la liberation de la dll" << std::endl;
    } 

    std::cout << "DLL libérée avec succes !" << std::endl;

    return 0;
}