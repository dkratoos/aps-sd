#include "Socket.h"
#include <process.h>
#include <string>

unsigned __stdcall Answer(void* a) {
  Socket* s = (Socket*) a;

  while (1) {
    std::string r = s->ReceiveLine();
    if (r.empty()) break;

      s->SendLine("Conectado ao servidor! ");
    if (r.compare("listarCandidatos") == 1) {
      s->SendLine("Listando candidatos");
    } if (r.compare("listarVotos") == 1) {
      s->SendLine("Listando os votos");
    } else {

    }
  }

  delete s;

  return 0;
}

int main(void) {
  SocketServer servidor(2000,5);

  while (1) {
    Socket* s = servidor.ValidarPorta();

    unsigned ret;
    _beginthreadex(0,0,Answer,(void*) s,0,&ret);
  }
 
  return 0;
}
