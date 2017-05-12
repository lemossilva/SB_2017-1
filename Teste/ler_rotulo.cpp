// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
using namespace std;

struct instr_format* create_instr(void);


typedef struct instr_format{
      string name; 
      int code,args;

    } instr_format;

int main () {
  	string line, rotulo;
  	ifstream myfile ("retangulo.asm");  //abre arquivo fonte
  	ofstream tab_simb ("Ts.txt");        //abre tabela de simbolos
  	int pos,i; //variaveis de controle

   

    struct instr_format *Instr, Instr2;

    Instr = create_instr();  



    for(i=0;i<14;i++){
      cout<< Instr[i].name<<" "<<Instr[i].code<<" "<<Instr[i].args<<endl;
    }

  	if (!tab_simb.is_open() || ! myfile.is_open()){  //verifica se arquivos foram abertos
  		cout<<"arquivo não abriu"<< endl;
  		return 0;
  	}

  	while ( getline (myfile,line) ){  //laço que percorre o arquivo fonte recolhendo linhas
      	//cout << line << '\n';
      	pos = (int)line.find(":");     // pega posição do dois pontos (:)


      	if(pos >0){  //será que pode usar isso? pois estou chegando se pos é diferente de string::npos
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


struct instr_format* create_instr(void){
  
  struct instr_format *Instr = (instr_format*)malloc(14*sizeof(instr_format));
  int i;

  for(i=0;i<14;i++){
    Instr[i].code = i+1;
    Instr[i].args = 2;
  }
  Instr[8].args = 3;
  Instr[13].args =1;

  Instr[0].name = "ADD";
  Instr[1].name = "SUB";
  Instr[2].name = "MULT";
  Instr[3].name = "DIV";
  Instr[4].name = "JMP";
  Instr[5].name = "JMPN";
  Instr[6].name = "JMPP";
  Instr[7].name = "JMPZ";
  Instr[8].name = "COPY";
  Instr[9].name = "LOAD";
  Instr[10].name = "STORE";
  Instr[11].name = "INPUT";
  Instr[12].name = "OUTPUT";
  Instr[13].name = "STOP";



  

  return Instr;
}


