#include <iostream>
#include <cstdlib>
#include <time.h>
#include <locale.h>
#include "Socket.h"

using namespace std;

void listarVotosCandidatos() {
	try {
		SocketClient s("localhost", PORTA_SERVIDOR);
		int length = 0;

		s.SendLine("listarVotos");

		while (1) {
			string res = s.ReceiveLine();

			if (res.empty()) {
				break;
			}

      cout << res << endl;
			length++;
			if (length == TAMANHO_LISTA) {
				break;
			}
		}
	}
	catch (const char *s) {
		cerr << s << endl;
	}
	catch (std::string s) {
		cerr << s << endl;
	}
	catch (...) {
		cerr << "unhandled exception\n";
	}
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");

	int operacao, voto;
	bool votou;

	cout << "\nInforme a operacao que deseja:\n1) Listar candidatos\n2) Sair\n\n";
	cin >> operacao;

	while (operacao != 2) {
		cout << "\n"
				 << endl;

		if (operacao == 1) {
			listarVotosCandidatos();
		}

		cout << "\nInforme a operacao que deseja:\n1) Listar candidatos\n2) Sair\n\n";
		cin >> operacao;
	}

	cout << "Bye\n";
}
