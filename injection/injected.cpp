#include <windows.h>
#include <iostream>

extern "C" {
    __declspec(dllexport) int Kysan(int x)  {
        std::cout << "dll function called !" << std::endl;
        return x + 1;
    }
}


DWORD WINAPI myHackThread(HMODULE dllHandle)
{
    // FreeConsole(); // peut échouer mais osef
    // AllocConsole();

    std::cout << "Hacked ! " << std::endl;

    // while(true) {
    //     if(GetAsyncKeyState('q') & 0x8000) {
    //         std::cout << "oui ?" << std::endl;
    //     }
    // }

    // j'apprend à faire une dll et je reviens dessus

    // pour fermer la dll à la fin
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
            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) myHackThread, DLLhandle, 0, nullptr));

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