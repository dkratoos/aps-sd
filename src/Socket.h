#include <WinSock2.h>

#include <string>

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
};
