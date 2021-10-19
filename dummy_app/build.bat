g++ -c -DBUILDING_DLL ./mylib/lib.cpp
g++ lib.o -shared -o ./out/mylib.dll
del lib.o
g++ main.cpp -L./out/ -lmylib -o ./out/app
