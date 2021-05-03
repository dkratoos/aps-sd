#include <iostream>
#include <cstdlib>
#include <time.h>
#include <locale.h>
#include "Socket.h"

using namespace std;

struct candidato {
	int numero;
	string sigla;
	string nome;
};

bool enviarVoto (int numero) {
  try {
    SocketClient s("localhost", PORTA_SERVIDOR);

    s.SendLine(std::to_string(numero));

    while (1) {
      string res = s.ReceiveLine();
      if (res.empty()) {
				break;
			}

      cout << res << endl;
			return res.compare("votou") == 1;
    }
  } catch (const char* s) {
    cerr << s << endl;
  } catch (std::string s) {
    cerr << s << endl;
  } catch (...) {
    cerr << "unhandled exception\n";
  }

	return false;
}

void listarCandidatos() {
	try {
		SocketClient s("localhost", PORTA_SERVIDOR);
		int length = 0;

		s.SendLine("listarCandidatos");

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

bool buscarCandidato(int numero) {
	try {
		SocketClient s("localhost", PORTA_SERVIDOR);

		s.SendLine("buscarCandidato:" + std::to_string(numero));

		while (1) {
			string res = s.ReceiveLine();

			if (res.empty()) {
				break;
			}

      cout << res << endl;

			if (res.compare("Candidato não existente, por favor informe outro número.") == 1) {
				return false;
			}

			if (res.rfind("Tem certeza que deseja votar no candidato", 0) == 1) {
				return true;
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

	return false;
}

bool votar(int numero) {
	buscarCandidato(numero);
	char operacao;

	cin >> operacao;

	if (operacao == 'S') {
		enviarVoto(numero);
		return true;
	}

	return false;
}

int main(int argc,char *argv[]){
	setlocale(LC_ALL, "");

	int operacao, voto;
	bool votou;

	cout << "\nInforme a operacao que deseja:\n1) Listar candidatos\n2) Enviar voto\n3) Sair\n\n";
	cin >> operacao;

	while (operacao != 3) {
		cout << "\n" << endl;

		if (operacao == 1) {
			listarCandidatos();
		}

		if (operacao == 2) {
			votou = false;

			while (votou == false) {
				cout << "Informe o número do candidato que você deseja votar." << endl;
				cin >> voto;
				votou = votar(voto);
			}

			cout << "\nVoto computado, obrigado." << endl;
		}
	
		cout << "\nInforme a operacao que deseja:\n1) Listar candidatos\n2) Enviar voto\n3) Sair\n\n";
		cin >> operacao;
	}

	cout << "Bye\n";
}
