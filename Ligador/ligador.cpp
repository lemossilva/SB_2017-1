/*//////////////////////////////////////////////////////////////////

LIGADOR - SOFTWARE BÁSICO - ASSEMBLY HIPOTÉTICO
COMPILAR COM G++11!!!!!

////////////////////////////////////////////////////////////////////*/

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
using namespace std;

bool linker(int nfile, vector<ifstream*> object_vector, ofstream &output)
{
	string lines, words;
	vector <vector<string> > table_use_g, table_gd_g, code_g;
	vector <vector<string> >::iterator vec_vec_it;
	vector <string>::iterator vec_it;
	string::iterator it;
	vector <vector <int> > realoc_vec;
	vector <int> correction_factor;

	for (int i = 0; i < nfile; ++i)			//Loop pelo número de arquivos objeto
	{
		vector<string> table_use, table_gd, code;
		vector<int> realocation;
		short int counter = 0;
		cout << "arquivo: " << i << endl;
		while(getline(*object_vector[i],lines))		//Percorre linha a linha o arquivo objeto
		{	
			if(lines.find("TABLE USE") != string::npos)		//Achou a tabela de uso
			{
				while(getline(*object_vector[i],lines) && lines.find("TABLE DEFINITION") == string::npos) //Enquanto ainda for a tabela de uso
				{
					istringstream is(lines);
					while(getline(is, words,' '))	//Pega as palavras dentro da linha
					{
						table_use.push_back(words);	//Coloca no vector
					}

				}
			}
			if(lines.find("TABLE DEFINITION") != string::npos)	//Funcionamento similar ao if anterior
			{
				while(getline(*object_vector[i],lines) && lines.find("TABLE REALOCATION") == string::npos)
				{
					istringstream is(lines);
					while(getline(is, words,' '))
					{
						table_gd.push_back(words);
						//cout << words << endl;
					}

				}
			}
			if(lines.find("TABLE REALOCATION") != string::npos)	//Acha a tabela com os bits de realocação
			{
				getline(*object_vector[i],lines);
				for(it = lines.begin(); it < lines.end(); ++it)
				{
					realocation.push_back(int(*it) - 48);		//Converte pra int e joga no vector
					//cout << (int(*it)-48) << endl;
					++counter;									//Conta o tamanho do código para calcular o fator de correção
				}	
			}
			if(lines.find("CODE") != string::npos)		//Acha a seção CODE
			{
				while(getline(*object_vector[i],lines, ' '))	//Separa a seção CODE por espaços
				{
					code.push_back(lines);		//Envia pro vector
				}
			}
		}

		if(i == 0)			//Calcula o fator de correção no segundo arquivo
		{
			correction_factor.push_back(0);
			correction_factor.push_back(counter);
		}
		else if(i == 1)		//Calcula o fator de correção no terceiro arquivo
		{
			correction_factor.push_back(counter+(correction_factor[1]));
		}

		if(i != 0)			//Aplica o fator de correção
		{
			for(int j = 1; j < table_gd.size(); j+=2) // Aplica o fator de correção na tabela de definições
			{
				table_gd[j] = to_string(stoi(table_gd[j]) + correction_factor[i]);
				//cout << table_gd[j] << endl;
			}
			for(int cont = 0; cont < code.size(); cont++) //Aplica na seção CODE dependendo da tabela de realocação
			{
				if(realocation[cont])
				{
					code[cont] = to_string(correction_factor[i] + stoi(code[cont]));
					//cout << code[cont] << endl;
				}
			}
		}
		table_use_g.push_back(table_use);		//Salva todas as informações do arquivo[i]
		realoc_vec.push_back(realocation);
		table_gd_g.push_back(table_gd);
		code_g.push_back(code);
	}

	for(int i = 0; i < table_use_g.size(); i++)		//Resolve pendências cruzadas
	{
		for(int j = 0; j < table_use_g[i].size(); j+=2)
		{
			//cout << table_use_g[i][j] << ' ';
			for(int g = 0; g < table_gd_g.size(); g++)
			{
				for(int k = 0; k < table_gd_g[g].size(); k+=2)
				{
					if(table_use_g[i][j] == table_gd_g[g][k])
					{
						//cout << table_gd_g[g][k] << "  achou" << endl;
						int temp = stoi(code_g[i][stoi(table_use_g[i][j+1])]) + stoi(table_gd_g[g][k+1]);
						code_g[i][stoi(table_use_g[i][j+1])] = to_string(temp);
						//cout << code_g[i][stoi(table_use_g[i][j+1])] << " conteudo " << endl;
					}
				}
			}
		}
	}
	for(vector <string> vec : code_g)	//Passa a saída pro arquivo de saída
	{
		for(string st : vec)
		{
			output << st << ' ';
		}
	}
	/*for(vector<string> vec : table_gd_g)
	{
		for(string st : vec)
		{
			cout << st << endl;
		}
	}*/
	//cout << table_use_g[0][1] << endl;
	//cout << realoc_vec[0][4] << endl;
	//cout << correction_factor[1] << endl;

	return output.good();
}

ifstream object_file, object_file2, object_file3;


int main(int argc, char const *argv[])
{
	vector<ifstream*> files;

	if(argc < 4 || argc > 5)		//Se forem 2 arquivos de entrada, argc = 4. Se forem 3 arquivos de entrada, argc = 5;
		cerr << "Número inválido de argumentos" << endl;

	if(argc == 4)					//Abre os arquivos
	{
		cout << argv[1] << endl;
		object_file.open(argv[1]);
		files.push_back(&object_file);
		object_file2.open(argv[2]);
		files.push_back(&object_file2);
	}
	else if(argc == 5)
	{
		object_file.open(argv[1])
		files.push_back(&object_file);
		object_file2.open(argv[2])
		files.push_back(&object_file2);
		object_file3.open(argv[3])
		files.push_back(&object_file3);
	}

	for(int i = 1; i < argc - 1; i++) //Confere se a extensão dos arquivos de entrada está correta
	{
		string filename(argv[i]);
		if(filename.substr(filename.find(".")+1) != "o")
			cerr << "Extensão errada para arquivo de entrada (default: .o)" << endl;
	}
	string filename(argv[argc-1]);	//Confere se a extensão do arquivo de saída está correta
	if(filename.substr(filename.find(".")+1) != "e")
		cerr << "Extensão errada para arquivo de saída (default: .e)" << endl;
	ofstream exe_file(argv[argc-1], ios::out|ios::trunc); //Abre o arquivo de saída



	linker((argc - 2), files, exe_file);  //Liga os arquivos

	return 0;
}