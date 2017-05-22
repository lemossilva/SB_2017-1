/*
##############################################

FUNÇÃO MAIN PARA O PRÉ-PROCESSADOR E MONTADOR

##############################################
*/
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
	if (!(argc == 4))							//Checa se colocou o número correto de argummentos
	{
		cerr << "Número inválido de argumentos" << endl;
		return -1;
	}
	filename = argv[2];
	option = argv[1];
	saida = argv[3];
	if ((filename.substr(filename.find(".")+1) == "asm") && option == "-o")  //Preprocessar e montar .asm
	{
		//Chama pré-processador e montador
		arquivo_entrada.open(argv[2]);
		if(!arquivo_entrada.is_open())
		{
			cerr << "Erro ao abrir arquivo" << endl;
			return -1;
		}
		arquivo_pre_temp.open(argv[3],ios::out|ios::trunc);
		if(!arquivo_pre_temp.is_open())
		{
			cerr << "Erro ao abrir arquivo" << endl;
			return -1;
		}
		pre_proc(arquivo_entrada,arquivo_pre_temp);
		chama_montador(saida,saida);
		remove(argv[3]);								//Exclui arquivo intermediário
	}
	else if ((filename.substr(filename.find(".")+1) == "pre") && option == "-o") //Apenas montar arquivo .pre
	{
		chama_montador(filename,saida);
	}
	else if((filename.substr(filename.find(".")+1) == "asm") && option == "-p") //Preprocessar arquivo .asm
	{
		string ext (".pre");
		arquivo_entrada.open(argv[2]);
		if(!arquivo_entrada.is_open())
		{
			cerr << "Erro ao abrir arquivo" << endl;
			return -1;
		}
		saida += ext;
		arquivo_pre_temp.open(saida.c_str(),ios::out|ios::trunc);
		if(!arquivo_pre_temp.is_open())
		{
			cerr << "Erro ao abrir arquivo" << endl;
			return -1;
		}
		pre_proc(arquivo_entrada,arquivo_pre_temp);
	}
	else
	{
		cerr << "Opção inválida para essa extensão!" << endl;
		return -1;
	}
	return 0;
}