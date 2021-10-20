#include <windows.h>
#include <iostream>


// extern "C" {
//     __declspec(dllexport) int Kysan(int x)  {
//         std::cout << "dll function called !" << std::endl;
//         return x + 1;
//     }
// }


int monHook(int x) {
    return 100;
}

void hookF(void *target, size_t size, void * hook) {

    // * target est un pointeur vers une fonction
    // * size est le nombre d'octet que prend cette fonction dans la mémoire
    // * hook est un pointeur vers une fonction (tout les appels vers target function seront redirigé vers la fonction hook)

    // rediriger la fonction on va simplement remplacer son bytecode par une instruction de jump 
    // qui va faire sauter le pointeur d'instruction sur la fonction de notre choix
    // soit:
    // jmp <adresseHook>
    // à noté
    // l'instruction jmp est relative
    // dans les faits ont fera donc plus 
    // jmp <distanceJusquaHook>
    // nous allons donc devoir calculer cette distance manuellement

    
    // quelque petits détails prérequis sur l'instruction JMP:
    // décompose de la manière suivante:
    // - 1byte d'OPcode (0xe9 pour JMP)
    // - 4 bytes d'adresses (l'adresse sur 32bits c'est logique)

    // les 4 bytes d'adresse correspondent à l'offset qui vas être ajouté au registre IP
    // à la fin de l'instruction JMP
    // il faut bien contabilisé la position du registre IP
    // car celui si ce trouvera à <targetFunction> + 1 (opcode) + (4 adresse)
    
    // l'erreur serait la suivante
    // si on fait le jump avec la différence brute entre target et hook 
    // serait une erreur
    // nous allons nous retrouver à <hook>+5
    // il faudra donc soustraire 5 à notre différence
    // voici donc la code qui réalise cela
    // (rappel distance de A à B = B - A)
    // donc distance de target à hook = hook - target; 
    
    
    
    uintptr_t offset = ((uintptr_t) hook - (uintptr_t)target) - 5;

    // petit log
    if (true) {
        std::cout << "adresse target : " << (void*) target << std::endl;
        std::cout << "adresse hook : " << (void*) hook << std::endl;
        std::cout << "offset entre les deux: " << (void*) offset << std::endl;
    }


    std::cout << "1" << std::endl;
    // ici on va simplement écrire dans la mémoire


    // on récupère les permissions pour écrire dans cette zone mémoire
    DWORD protectionState;
    VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &protectionState);
    memset(target, 0x90, 0x5);


    *((BYTE*)target) = 0xE9;
    *(uintptr_t*)((uintptr_t)target+1) = offset; 

    DWORD temp;
    VirtualProtect(target, size, protectionState, &temp);
}

DWORD WINAPI hackingThread(HMODULE dllHandle)
{
    std::cout << "hacking thread started" << std::endl;
    // * taille en octet de la fonction dans le binaire
    size_t funcSize = 0x32;
    
    HMODULE myLibModule = GetModuleHandleW(L"mylib.dll");
    void * myFunction = (void *) GetProcAddress(myLibModule, "Kysan");

    void * diff = (void *) ((uintptr_t) myFunction - (uintptr_t) myLibModule);
    std::cout << "adresse module :"             << (void*) myLibModule  << std::endl;
    std::cout << "adresse Kysan() :"            << (void*) myFunction   << std::endl;
    std::cout << "offset fonction Kysan() :"    << (void*) diff         << std::endl;

    hookF(myFunction, funcSize, (void *)monHook);
    std::cout << "hook done !" << std::endl;

    // FreeConsole();
    // AllocConsole();
    

    // memcpy(buffer, )

    // while(true) {
    //     if(GetAsyncKeyState('q') & 0x8000) {
    //         std::cout << "oui ?" << std::endl;
    //     }

    // }



    CloseHandle(myLibModule);
    CloseHandle(dllHandle);
    return 0;
}
 
// https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain
// * cette fonction va s'executer quand notre DLL va être chargé en mémoire

BOOL WINAPI DllMain(
    HINSTANCE DLLhandle,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.


            // le processus viens donc de charger notre dll
            // on crée donc un thread
            // https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
            
            
            // pour qu'il se ferme à la fin du processus
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) hackingThread, DLLhandle, 0, nullptr));

            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;
}