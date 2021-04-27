g++ -c  ./src/Socket.cpp       -o Socket.o
g++ ./src/ClienteTipo2.cpp     Socket.o -lwsock32 -o ClienteTipo2.exe
g++ ./src/Servidor.cpp       Socket.o -lwsock32 -o Servidor.exe
