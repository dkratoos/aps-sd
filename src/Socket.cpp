#include "Socket.h"
#include <iostream>

#define TAMANHO_LISTA 3

using namespace std;

int Socket::nofSockets_= 0;

struct candidato {
  int numero = 0;
  int qtdVotos = 0;
  string sigla;
  string nome;
} candidatos[TAMANHO_LISTA];

Socket* SocketServer::MockCandidatos () {
  candidatos[0].numero = 17;
	candidatos[0].sigla = "PCDOB";
	candidatos[0].nome = "Eneias";

	candidatos[1].numero = 13;
	candidatos[1].sigla = "PT";
	candidatos[1].nome = "Poste";

	candidatos[2].numero = 22;
	candidatos[2].sigla = "PSL";
	candidatos[2].nome = "Laranja";
}

void SocketServer::ComputarVoto(int numero) {
	for(int i = 0; i < TAMANHO_LISTA; i++){
		if (candidatos[i].numero == numero) {
      candidatos[i].qtdVotos += 1;
      this->SendLine("votou");
		}
	}
}

void SocketServer::BuscarCandidato(int numero) {
	candidato cand;
	cand.nome = "";

	for(int i = 0; i < TAMANHO_LISTA; i++){
		if (candidatos[i].numero == numero) {
			cand = candidatos[i];
		}
	}

	if (cand.nome.compare("") == 0) {
		this->SendLine("\nCandidato não existente, por favor informe outro número.\n");
	} else {
    this->SendLine("\nTem certeza que deseja votar no candidato: " + cand.nome + "|" + cand.sigla + "? S/N");
  }
}

void SocketServer::ListarCandidatos() {
	for(int i = 0; i < TAMANHO_LISTA; i++){
		string data = std::to_string(i + 1) + " - Candidato -> " + candidatos[i].nome + " - " + std::to_string(candidatos[i].numero) + "|" + candidatos[i].sigla;

    this->SendLine(data);
	}
}

void SocketServer::ListarVotos() {
	for(int i = 0; i < TAMANHO_LISTA; i++){
		string data = std::to_string(i + 1) + " - Candidato -> " + candidatos[i].nome + " - " + std::to_string(candidatos[i].numero) + "|" + candidatos[i].sigla + " - " + std::to_string(candidatos[i].qtdVotos) + " Votos.";

    this->SendLine(data);
	}
}

void Socket::Start() {
  if (!nofSockets_) {
    WSADATA info;
    if (WSAStartup(MAKEWORD(2,0), &info)) {
      throw "Could not start WSA";
    }
  }
  ++nofSockets_;
}

void Socket::End() {
  WSACleanup();
}

Socket::Socket() : s_(0) {
  Start();
  // UDP: use SOCK_DGRAM instead of SOCK_STREAM
  s_ = socket(AF_INET,SOCK_STREAM,0);

  if (s_ == INVALID_SOCKET) {
    throw "INVALID_SOCKET";
  }

  refCounter_ = new int(1);
}

Socket::Socket(SOCKET s) : s_(s) {
  Start();
  refCounter_ = new int(1);
};

Socket::~Socket() {
  if (! --(*refCounter_)) {
    Close();
    delete refCounter_;
  }

  --nofSockets_;
  if (!nofSockets_) End();
}

Socket::Socket(const Socket& o) {
  refCounter_=o.refCounter_;
  (*refCounter_)++;
  s_         =o.s_;

  nofSockets_++;
}

Socket& Socket::operator=(Socket& o) {
  (*o.refCounter_)++;

  refCounter_=o.refCounter_;
  s_         =o.s_;

  nofSockets_++;

  return *this;
}

void Socket::Close() {
  closesocket(s_);
}

std::string Socket::ReceiveLine() {
  std::string ret;
  while (1) {
    char r;

    switch(recv(s_, &r, 1, 0)) {
      case 0: 
        return ret;
      case -1:
        return "";
    }

    ret += r;
    if (r == '\n')  return ret;
  }
}

void Socket::SendLine(std::string s) {
  s += '\n';
  send(s_,s.c_str(),s.length(),0);
}

SocketServer::SocketServer(int port, int connections, TypeSocket type) {
  sockaddr_in sa;

  memset(&sa, 0, sizeof(sa));

  sa.sin_family = PF_INET;             
  sa.sin_port = htons(port);          
  s_ = socket(AF_INET, SOCK_STREAM, 0);
  if (s_ == INVALID_SOCKET) {
    throw "INVALID_SOCKET";
  }

  if(type==NonBlockingSocket) {
    u_long arg = 1;
    ioctlsocket(s_, FIONBIO, &arg);
  }

  /* bind the socket to the internet address */
  if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
    closesocket(s_);
    throw "INVALID_SOCKET";
  }
  
  listen(s_, connections);  
  cout << "Servidor iniciado na porta: " << port;                             
}

Socket* SocketServer::ValidarPorta() {
  SOCKET new_sock = accept(s_, 0, 0);
  if (new_sock == INVALID_SOCKET) {
    int rc = WSAGetLastError();
    if(rc==WSAEWOULDBLOCK) {
      return 0; // non-blocking call, no request pending
    }
    else {
      throw "Invalid Socket";
    }
  }

  Socket* r = new Socket(new_sock);
  return r;
}

SocketClient::SocketClient(const std::string& host, int port) : Socket() {
  std::string error;

  hostent *he;
  if ((he = gethostbyname(host.c_str())) == 0) {
    error = strerror(errno);
    throw error;
  }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr = *((in_addr *)he->h_addr);
  memset(&(addr.sin_zero), 0, 8); 

  if (::connect(s_, (sockaddr *) &addr, sizeof(sockaddr))) {
    error = strerror(WSAGetLastError());
    throw error;
  }
}
