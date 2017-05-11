// reading a text file
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main () {
  	string line, rotulo;
  	ifstream myfile ("retangulo.asm");  //abre arquivo fonte
  	ofstream tab_simb ("Ts.txt");        //abre tabela de simbolos
  	int pos,i; //variaveis de controle


  	if (!tab_simb.is_open() || ! myfile.is_open()){  //verifica se arquivos foram abertos
  		cout<<"arquivo não abriu"<< endl;
  		return 0;
  	}

  	while ( getline (myfile,line) ){  //laço que percorre o arquivo fonte recolhendo linhas
      	cout << line << '\n';
      	pos = line.find(":");     // pega posição do dois pontos (:)


      	if(pos >0){
      		for(i=0;i<pos;i++){   //for pega todos os caracteres anteriores a : e salva na tabela de simbolos
      			rotulo = line.at(i);
      			tab_simb<<rotulo;
      		}
      			tab_simb<<endl;



  	  	}


    }

	myfile.close();
    tab_simb.close();

  	return 0;
}
