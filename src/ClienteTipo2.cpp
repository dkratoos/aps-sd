#include "Socket.h"

#include <iostream>

using namespace std;

int main(void) {
  try {
    SocketClient s("localhost", 2000);

    s.SendLine("listarCandidatos");

    while (1) {
      string l = s.ReceiveLine();
      if (l.empty()) break;
      cout << l;
      cout.flush();
    }
  } catch (const char* s) {
    cerr << s << endl;
  } catch (std::string s) {
    cerr << s << endl;
  } catch (...) {
    cerr << "unhandled exception\n";
  }

  return 0;
}

