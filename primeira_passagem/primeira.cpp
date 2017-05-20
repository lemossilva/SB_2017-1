// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

typedef struct sep_instr{
      string rotulo,op,mem1,mem2;
      void reset() {
        rotulo.clear();
        op.clear();
        mem1.clear();
        mem2.clear();
    }
} sep_instr;

typedef struct st_tab_simb{
      string rotulo;
      int ender;
} st_tab_simb;

struct sep_instr separa_linha(string,int*);

void mostra_instrucao(string,string,string,string);

int main (int argv, char** argc) {

  string line,rotulo,op,mem1,mem2;
  int contador_ts=0,*contador_mem = (int*)calloc(1,sizeof(int));
  struct sep_instr instruc;
  struct st_tab_simb *tabela_simb = (st_tab_simb*)malloc(8*sizeof(st_tab_simb));

	/** VERIFICA NUMERO ARGUMENTOS*/
	if(argv <2){  
      cout<<"numero de arquivos invalidos"<<endl;
      return 1;
  }

    /*** ARQUIVOS   ***/
  ifstream myfile (argc[1]);  //abre arquivo fonte
  ofstream fp_tab_simb ("Ts.txt");        //abre tabela de simbolos

  //verifica se arquivos foram abertos
  if (!fp_tab_simb.is_open() || ! myfile.is_open()){  
    cout<<"arquivo não abriu"<< endl;
    return 1;
  }

  /* Enquanto arquivo não chegar ao fim, pegue uma linha*/
  while ( getline (myfile,line) ){
    
    

    /*** FUNCAO QUE SEPARA ELEMENTOS LINHA **/
    instruc = separa_linha(line,contador_mem);

    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;

    mostra_instrucao(rotulo,op,mem1,mem2);


    if(!rotulo.empty()){
      //procura tabela de simbolos
      //if(se achou){
        //error;
      //}else{
        //insere
        tabela_simb[contador_ts].rotulo = rotulo;
        tabela_simb[contador_ts].ender = *contador_mem;
        contador_ts++;
       //}
    }

    
    



    op.clear();
    mem1.clear();
    rotulo.clear();
    mem2.clear();


  }
  
  int i;
  cout<<endl<<"tabela de simbolos:"<<endl;
  for(i=0;i<4;i++){
    cout<<tabela_simb[i].rotulo<<" "<<tabela_simb[i].ender<<endl;
  }


  myfile.close();
  fp_tab_simb.close();

  return 0;
}

struct sep_instr separa_linha(string line,int * contador_mem){

  struct sep_instr instruc;

  int pos;


   pos = (int)line.find(":");
    if(pos > 0){
      instruc.rotulo.assign(line,0,pos);
      line.erase(0,pos+2); //eliminar também o espaço após ':'
    }

    /**PROCURA OPERAÇÃO**/
    pos = (int)line.find(" "); //espaço para pegar codigo de instrução
    if(pos>0){
      instruc.op.assign(line,0,pos);  // pega codigo de instrução
      line.erase(0,pos+1); //+1 para eliminar espaço
      *contador_mem = *contador_mem +1;
    }else{ //if(op.empty()){
      /*caso do STOP e SPACE*/
      instruc.op=line;
      line.erase(0,line.length());
      *contador_mem = *contador_mem +1;
      //return caso faça função 
    }

    /** PRIMEIRO OPERANDO **/
    pos = (int)line.find(",");
    if(pos>0 ){
      instruc.mem1.assign(line,0,pos);
      line.erase(0,pos+1); //ja elimina o ;
      *contador_mem = *contador_mem +1;

      if(line.length()>0){  //segundo operando;
        instruc.mem2 = line;
        *contador_mem = *contador_mem +1;
      }
    }
    else if(line.length()>0){
      instruc.mem1 = line;
      line.erase(0,line.length());
      *contador_mem = *contador_mem +1;
    }

    //*contador_mem = 2;

  return instruc;
}

void mostra_instrucao(string rotulo,string op,string mem1,string mem2){

  if(!rotulo.empty())
    cout<<"rotulo: "<<rotulo<<" , ";

  cout<<"instrução: "<<op;
  if(!mem1.empty())
    cout<<" , mem1: "<<mem1;
  if(!mem2.empty())
    cout<<" , mem2: "<<mem2;

  cout<<endl;

  return;
}
