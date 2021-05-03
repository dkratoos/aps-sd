#include "Socket.h"
#include <process.h>
#include <string>

unsigned __stdcall Answer(void* a) {
  SocketServer* server = (SocketServer*) a;

  while (1) {
    std::string payload = server->ReceiveLine();

    if (payload.empty()) {
      break;
    } else {
      if (payload.compare("listarCandidatos") == 1) {
        server->ListarCandidatos();
      } else if (payload.compare("listarVotos") == 1) {
        server->ListarVotos();
      } else if (payload.rfind("buscarCandidato", 0) == 0) {
        std::string token = payload.substr(payload.find(":") + 1);
        server->BuscarCandidato(stoi(token));
      } else {
        server->ComputarVoto(stoi(payload));
      }
    }
  }

  delete server;

  return 0;
}

int main(void) {
  SocketServer servidor(PORTA_SERVIDOR, 5);

  while (1) {
    Socket* s = servidor.ValidarPorta();
    servidor.MockCandidatos();

    unsigned ret;
    _beginthreadex(0,0,Answer,(void*) s,0,&ret);
  }
 
  return 0;
}
