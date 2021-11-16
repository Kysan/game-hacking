#include <iostream>
#include <windows.h>
#include <map>


#include <psapi.h> // 

int getProcessPIDByName(std::string name) {

}

// (cmd.exe) -> 1337
std::map<std::string, int> listAllProcess() {
    std::map<std::string, int> out;
    DWORD process[10240], cbNeeded, cProcess;
    unsigned int i;

    if(!EnumProcesses(process, sizeof(process), &cbNeeded)) {
        return 0;
    }

    
}


int main() {

    HANDLE processH = OpenProcess();

    return 0;
}