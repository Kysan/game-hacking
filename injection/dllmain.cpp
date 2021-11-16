#include <windows.h>
#pragma comment(lib, "user32.lib")
#include <iostream>
#include <vector>

#include "Hooking.hpp"
#include "hooks.hpp"

DWORD WINAPI hackingThread(HMODULE dllHandle)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout << "Hooking thread started" << std::endl;

    HMODULE GameAssembly = GetModuleHandleW(L"GameAssembly.dll");

    std::cout << "game assembly module base adresse : " << (void *)(GameAssembly);
    void *oFeatureAccessible = (uint64_t *)GameAssembly + 0x00895880;
    std::cout << "featureAccessible : " << (void *)oFeatureAccessible << std::endl;
    size_t sKysan = 0x32;
    hook64(oFeatureAccessible, 0x32, (void *)hfeatureAccessible, "featureAccessible", false);

    while (true)
    {
        if (GetAsyncKeyState('q'))
        {
            std::cout << "oui ?" << std::endl;
        }
    }

    // // CloseHandle(myLibModule);
    CloseHandle(dllHandle);
    return 0;
}

// https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain
// * cette fonction va s'executer quand notre DLL va être chargé en mémoire

BOOL WINAPI DllMain(
    HINSTANCE DLLhandle, // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)   // reserved
{

    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.

        // le processus viens donc de charger notre dll
        // on crée donc un thread
        // https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread

        // pour qu'il se ferme à la fin du processus
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hackingThread, DLLhandle, 0, nullptr));

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