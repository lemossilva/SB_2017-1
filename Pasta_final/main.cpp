//ESQUELETO DA MAIN PARA O MONTADOR/PREPROCESSSADOR


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "preprocessador.h"
#include "montador.h"

using namespace std;

ifstream arquivo_entrada;
fstream arquivo_saida, arquivo_pre_temp;

int main(int argc, char const *argv[])
{
	string filename, option, saida, pre_temp;
	if (!(argc == 4))
	{
		cerr << "Número inválido de argumentos" << endl;
		return -1;
	}
	filename = argv[2];
	option = argv[1];
	saida = argv[3];
	if ((filename.substr(filename.find(".")+1) == "asm") && option == "-o")
	{
		//Chama pré-processador e montador
		cout << "ASM e -o" << endl;
		arquivo_entrada.open(argv[2]);
		arquivo_pre_temp.open(argv[3],ios::out|ios::trunc);
		pre_proc(arquivo_entrada,arquivo_pre_temp);
		chama_montador(saida,saida);
		remove(argv[3]);
	}
	else if ((filename.substr(filename.find(".")+1) == "pre") && option == "-o")
	{
		cout << "PRE e -o" << endl;
		chama_montador(filename,saida);
	}
	else if((filename.substr(filename.find(".")+1) == "asm") && option == "-p")
	{
		cout << "ASM e -p" << endl;
		string ext (".pre");
		arquivo_entrada.open(argv[2]);
		saida += ext;
		arquivo_pre_temp.open(saida.c_str(),ios::out|ios::trunc);
		pre_proc(arquivo_entrada,arquivo_pre_temp);
	}
	else
	{
		cerr << "Opção inválida para essa extensão!" << endl;
		return -1;
	}
	return 0;
}