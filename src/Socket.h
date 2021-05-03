#include <WinSock2.h>
#include <string>

#define TAMANHO_LISTA 3
#define PORTA_SERVIDOR 2000

enum TypeSocket {BlockingSocket, NonBlockingSocket};

class Socket {
  public:
    virtual ~Socket();
    Socket(const Socket&);
    Socket& operator=(Socket&);

    std::string ReceiveLine();

    void   Close();
    void   SendLine (std::string);

  protected:
    friend class SocketServer;

    Socket(SOCKET s);
    Socket();

    SOCKET s_;

    int* refCounter_;

  private:
    static void Start();
    static void End();
    static int  nofSockets_;
};

class SocketClient : public Socket {
  public:
    SocketClient(const std::string& host, int port);
};

class SocketServer : public Socket {
  public:
    SocketServer(int port, int connections, TypeSocket type=BlockingSocket);

    Socket* ValidarPorta();
    Socket* MockCandidatos();
    void ComputarVoto(int numero);
    void ListarCandidatos();
    void BuscarCandidato(int numero);
    void ListarVotos();
};
