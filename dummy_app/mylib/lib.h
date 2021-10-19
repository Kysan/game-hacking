
#ifndef MY_LIB_H
#define MY_LIB_H



/*

lib pour faire des expérimentations

*/


#ifdef BUILDING_DLL
#define DLL_EXPORT_OR_IMPORT __declspec(dllexport)
#else
#define DLL_EXPORT_OR_IMPORT __declspec(dllimport)
#endif

extern "C" {
    DLL_EXPORT_OR_IMPORT int Kysan(int x);
}

#endif  