g++ -c  ./src/Socket.cpp       -o Socket.o
g++ ./src/Eleitor.cpp     Socket.o -lwsock32 -o Eleitor.exe
g++ ./src/Admin.cpp     Socket.o -lwsock32 -o Admin.exe
g++ ./src/Servidor.cpp       Socket.o -lwsock32 -o Servidor.exe
