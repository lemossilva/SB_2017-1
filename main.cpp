//ESQUELETO DA MAIN PARA O MONTADOR/PREPROCESSSADOR


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
#include "preprocessador.h"
#include "montador"
using namespace std;

int main(int argc, char const *argv[])
{
	string filename, option;
	if (!(argc == 4))
	{
		cerr << "Número inválido de argumentos" << endl;
		return -1;
	}
	argv[2] >> filename;
	argv[1] >> option;
	if ((filename.substr(filename.find(".")+1) == "asm") && (option == "-o" || option == "-p"))
	{
		//Chama pré-processador e montador
	}
	else if ((filename.substr(filename.find(".")+1) == "pre") && option == "-p")
	{
		//Chama apenas o montador
	}
	else
	{
		cerr << "Opção inválida para essa extensão!" << endl;
		return -1;
	}
	return 0;
}