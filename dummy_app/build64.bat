g++ -c -march=x86-64 -m64 -DBUILDING_DLL ./mylib/lib.cpp
g++ lib.o -shared -march=x86-64 -m64  -o ./out/mylib.dll
del lib.o
g++ main.cpp -march=x86-64 -m64 -L./out/ -lmylib -o ./out/app
