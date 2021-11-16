#ifndef hooking_h
#define hooking_h

#include <map>
#include <windows.h>
#include <iostream>

std::map<std::string, void *> backup;

void *AllocatePageNearAddress(void *targetAddr)
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    const uint64_t PAGE_SIZE = sysInfo.dwPageSize;

    uint64_t startAddr = (uint64_t(targetAddr) & ~(PAGE_SIZE - 1)); //round down to nearest page boundary
    uint64_t minAddr = min(startAddr - 0x7FFFFF00, (uint64_t)sysInfo.lpMinimumApplicationAddress);
    uint64_t maxAddr = max(startAddr + 0x7FFFFF00, (uint64_t)sysInfo.lpMaximumApplicationAddress);

    uint64_t startPage = (startAddr - (startAddr % PAGE_SIZE));

    uint64_t pageOffset = 1;
    while (1)
    {
        uint64_t byteOffset = pageOffset * PAGE_SIZE;
        uint64_t highAddr = startPage + byteOffset;
        uint64_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

        bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

        if (highAddr < maxAddr)
        {
            void *outAddr = VirtualAlloc((void *)highAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (outAddr)
                return outAddr;
        }

        if (lowAddr > minAddr)
        {
            void *outAddr = VirtualAlloc((void *)lowAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (outAddr != nullptr)
                return outAddr;
        }

        pageOffset++;

        if (needsExit)
        {
            break;
        }
    }

    return nullptr;
}

void writeAbsuteJMP(void *buffer, void *destination)
{
    uint8_t instructions[13] = {
        0x49, 0xBA,                                     // mov  r10,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //      <nullptr>
        0x41, 0xFF, 0xE2                                // jmp r10
    };
    memcpy(instructions + 2, &destination, sizeof(destination));
    memcp(destination, &instructions, sizeof(instructions));
}

// * fait une copie de la fonction cible dans la mémoire
void backupFunction(void *target, size_t size, std::string name)
{
    void *funcAdr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy(funcAdr, target, size);

    backup.insert(std::pair<std::string, void *>(name, funcAdr));
}

int getPEArch()
{
    return sizeof(void *) == 4 ? 32 : 64;
}

// * target : fonction ciblée
// * size   : nombre d'octets que prend la fonction à hook en mémoire
// * hook   : pointeur vers la fonction ou l'on va détourner le flux d'execution
void hook64(void *target, size_t size, void *hook, std::string name, bool shouldBackupFunction)
{

    // (1) - on ~calcul les instructions à injecter
    // remplace <nullptr> par <adresse hook>

    if (size < sizeof(instructions))
    {
        std::cout << "Impossible de hook la fonction: " << name << "() !" << std::endl
                  << "motif:- Pas assez de place." << std::endl;
        return;
    }

    // (2) - backup de la fonction que l'on va écraser (optionnel)
    // (tu peux skip cette étape si tu veux comprendre la fonction)
    if (shouldBackupFunction)
    {
        backupFunction(target, size, name);
    }

    // (3)
    // changement et sauvegarde des anciennes permissions
    DWORD permBackup;
    VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &permBackup);

    memcpy(target, instructions, sizeof(instructions));

    // rétablissement des permissions
    VirtualProtect(target, size, permBackup, nullptr);
}

// * target : fonction ciblée
// * size   : nombre d'octets que prend la fonction à hook en mémoire
// * hook   : pointeur vers la fonction ou l'on va détourner le flux d'execution
void hook32(void *target, size_t size, void *hook, std::string name, bool shouldBackupFunction)
{
    // (1) - on ~calcul les instructions à injecter
    uint8_t instructions[5] = {
        0xe9,                  // jmp
        0x00, 0x00, 0x00, 0x00 // adresse relative entre target()  hook()
    };

    uint32_t offset = (uint32_t)hook - (uint32_t)target - sizeof(instructions);

    memcpy(instructions + 1, &offset, sizeof(offset));
    if (size < sizeof(instructions))
    {
        std::cout << "Impossible de hook la fonction: " << name << "() !" << std::endl
                  << "motif:- Pas assez de place." << std::endl;
        return;
    }
    // (2) - backup de la fonction que l'on va écraser (optionnel)
    // (tu peux skip cette étape si tu veux comprendre la fonction)
    if (shouldBackupFunction)
    {
        backupFunction(target, size, name);

        // (3)
        // changement et sauvegarde des anciennes permissions
        DWORD permBackup;
        VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &permBackup);

        memcpy(target, instructions, sizeof(instructions));
        // rétablissement des permissions
        VirtualProtect(target, size, permBackup, nullptr);
    }
}

#endif