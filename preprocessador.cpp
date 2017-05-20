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
	bool print;
	string lines, line_temp;
	stringstream temp, temp1, temp2, temp3;
	vector<string> equ_list, equ_cont_list;
	vector<string>::iterator equ_list_it, equ_cont_list_it;
	while(getline(srce_file,lines))
	{
		transform(lines.begin(),lines.end(), lines.begin(), ::toupper);
		if (lines.find(";") != string::npos)
		{
			temp2 << lines.substr(0,int(lines.find(";"))) << endl;	
		}
		else
		{
			temp2 << lines << endl;
		}
		//cout << temp2.str();
		

		if (temp2.str().find("EQU") != string::npos)
		{
			temp << temp2.str().substr(0,int(temp2.str().find(":")));
			equ_list.push_back(temp.str());
			temp.str("");
			temp1 << temp2.str().substr((temp2.str().find("EQU")+4),temp2.str().find("\n"));
			equ_list.push_back(temp1.str());
			temp1.str("");
		}
		else if(!equ_list.empty())
		{
			for(equ_list_it = equ_list.begin();	equ_list_it != equ_list.end(); equ_list_it+=2)
			{	
				if(lines.find(*equ_list_it) != string::npos)
				{
					temp3 << temp2.str().substr(0, temp2.str().find(*equ_list_it));
					temp2.str("");
					temp2 << temp3.str();
					temp3.str("");
					advance(equ_list_it,1);
					temp2 << *equ_list_it;
					advance(equ_list_it,-1);
				}
			}
		}
		if (temp2.str().find("EQU") == string::npos) //Remove as linhas que tem diretiva EQU
			no_comments << temp2.str();
		temp2.str("");
	}

	for(equ_list_it = equ_list.begin();	equ_list_it != equ_list.end(); equ_list_it++)
				cout << *equ_list_it; //PRINTAR A LISTA COM OS EQU
	//cout << endl;
	/*for(equ_cont_list_it = equ_cont_list.begin();	equ_cont_list_it != equ_cont_list.end(); equ_cont_list_it++)
				cout << *equ_cont_list_it << " "; //PRINTAR A LISTA COM OS VALORES DOS EQU*/

	return srce_file.eof() && no_comments.good();
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


