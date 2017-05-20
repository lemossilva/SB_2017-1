#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <list>
#include <sstream>
#include <vector>
using namespace std;


bool fancy(ifstream &srce_file, fstream &no_comments)
{

	string lines;
	stringstream temp, temp1, temp2, temp3;
	vector<string> equ_list;
	vector<string>::iterator equ_list_it;


	while(getline(srce_file,lines))
	{
		transform(lines.begin(),lines.end(), lines.begin(), ::toupper); //Coloca tudo em caixa alta
		if (lines.find(";") != string::npos)						// Retira comentários
		{
			temp2 << lines.substr(0,int(lines.find(";"))) << endl;	
		}
		else					//Se não houver comentários, escreve a linha
		{
			temp2 << lines << endl;
		}
		//cout << temp2.str();
		

		if (temp2.str().find("EQU") != string::npos)	//Acha linhas com a diretiva EQU
		{
			temp << temp2.str().substr(0,int(temp2.str().find(":")));	//Pega o label da linha com EQU
			equ_list.push_back(temp.str());		//Salva o label no vector
			temp.str("");						//Limpa a string temporária
			temp1 << temp2.str().substr((temp2.str().find("EQU")+4),temp2.str().find("\n")); //Pega o valor do EQU
			equ_list.push_back(temp1.str()); //Adiciona o valor do EQU na posição seguinte do label no vector
			temp1.str("");
		}
		else if(!equ_list.empty())			//Se a lista de EQU tiver algo, verifica se cada linha possui os labels já encontrados
		{
			for(equ_list_it = equ_list.begin();	equ_list_it != equ_list.end(); equ_list_it+=2) //Percorre de 2 em 2 pra procurar apenas os labels e não os valores
			{	
				if(lines.find(*equ_list_it) != string::npos)
				{
					temp3 << temp2.str().substr(0, temp2.str().find(*equ_list_it)); //Remove o nome do label da string
					temp2.str("");
					temp2 << temp3.str();
					temp3.str("");
					advance(equ_list_it,1);			//Substitui a label pelo valor definido na EQU
					temp2 << *equ_list_it;
					advance(equ_list_it,-1);		//Retorna o iterator para não quebrar o for e não dar segfault
				}
			}
		}
		if (temp2.str().find("EQU") == string::npos) //Remove as linhas que tem diretiva EQU
			no_comments << temp2.str();
		temp2.str("");
	}

	/*for(equ_list_it = equ_list.begin();	equ_list_it != equ_list.end(); equ_list_it++)
				cout << *equ_list_it; //PRINTAR A LISTA COM OS EQU */

	return srce_file.eof() && no_comments.good(); //Se deu tudo certo, retorna true
}

int main(int argc, char const *argv[])
{
	ifstream my_file("triangulo.asm");
	fstream file_out("saida.txt", ios::out|ios::in);
	fancy(my_file,file_out);
	my_file.close();
	file_out.close();
	return 0;
}


