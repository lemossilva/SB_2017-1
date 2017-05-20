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

typedef struct diret_format{
      string rotulo;
      int args,tam;

} diret_format;

typedef struct instr_format{
      string name;
      int code,tam;

} instr_format;

struct sep_instr separa_linha(string);

struct instr_format* create_instr(struct instr_format*);

struct diret_format* create_diret(struct diret_format*);


int procura_t_inst(struct instr_format*, string,int);

int  procura_tdir(diret_format*,string,int);

/** função que procura rotulo na tabela de simbolos, recebe tabela,rotulo e tamanho da tabela */
int procura_ts(st_tab_simb*, string,int);

/* recebe diretiva, retorna seu tamanho alocado*/
int executa_diretiva(string,string);

void mostra_instrucao(string,string,string,string);

int main (int argv, char** argc) {

  string line,rotulo,op,mem1,mem2;
  int contador_ts=0,contador_mem =0, tam=-1,acha_diret=-1,existe_rotul=1,tamanho_diret=0;


  struct sep_instr instruc;
  struct st_tab_simb *tabela_simb = NULL,*tab_temp=NULL;

  //struct instr_format *Instr =NULL; //tabela de instruções
  struct instr_format *Instr = (instr_format*)malloc(14*sizeof(instr_format));
  struct diret_format *Diret = (diret_format*)malloc(9*sizeof(diret_format));

  if(Instr ==NULL ||Diret ==NULL){
    cout<<"falha na alocacao"<<endl;
    exit(1);
  }

  Instr = create_instr(Instr);  //cria tabela de instruções
  Diret = create_diret(Diret);  //cria tabela de instruções



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


  tabela_simb = (st_tab_simb*)realloc(tab_temp,20*sizeof(st_tab_simb));
  if(tabela_simb == NULL){
          cout<<"falha na alocacao"<<endl;
          exit(1);
  }

  /* Enquanto arquivo não chegar ao fim, pegue uma linha*/
  /********************************/
  while ( getline (myfile,line) ){
    
    /*** FUNCAO QUE SEPARA ELEMENTOS LINHA **/
    instruc = separa_linha(line);

    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;

    mostra_instrucao(rotulo,op,mem1,mem2);

    
    /** VERIFICA SE EXISTE ROTULO **/
    if(!rotulo.empty()){
      /***procura por rotulo na TS**/
      existe_rotul = procura_ts(tabela_simb,rotulo,contador_ts);
      if(existe_rotul==1){
        cout<<"error: rotulo existente na tabela de definição"<<endl;
        exit(1);
      }
      else{
        cout<<"nao existe rotulo"<<endl;
        tabela_simb[contador_ts].rotulo = rotulo; //insere rotulo na tabela de simbolos
        tabela_simb[contador_ts].ender = contador_mem;// insere endereço (pos) do rotulo
        contador_ts++;
      }
    }

    //*procura por intrução na tabela de instruções **/
    tam = procura_t_inst(Instr,op,2);
    //*** achou instrução na tabela**/
    if(tam>0){
      contador_mem = contador_mem+tam;   // ****  incrementa contador de posição com tamanho da instrução
    }else{
      //**** procura na tabela de diretivas**//
      acha_diret = procura_tdir(Diret,op,2);
      if(acha_diret>=0){ ///ACHOOOU
        //*executa diretiva*//
        tamanho_diret = executa_diretiva(op,mem1);
        contador_mem= contador_mem + tamanho_diret;
      }
      else{
        cout<<"error: operação não identificada!!";
        exit(1);
      }
      
    }

    op.clear();
    mem1.clear();
    rotulo.clear();
    mem2.clear();

  }
  

  /** MOSTRAR TABELA DE SIMBOLOS **/
  int i;
  fp_tab_simb<<"TABLE USE"<<endl;
  for(i=0;i<contador_ts;i++){
    fp_tab_simb<<tabela_simb[i].rotulo<<" "<<tabela_simb[i].ender<<endl;
  }


  myfile.close();
  fp_tab_simb.close();

  free(Instr);
  free(Diret);
  free(tabela_simb);
  //free(contador_mem);


  return 0;
}




struct sep_instr separa_linha(string line){

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
      //*contador_mem = *contador_mem +1;
    }else{ //if(op.empty()){
      /*caso do STOP e SPACE*/
      instruc.op=line;
      line.erase(0,line.length());
      //*contador_mem = *contador_mem +1;
      //return caso faça função 
    }

    /** PRIMEIRO OPERANDO **/
    pos = (int)line.find(",");
    if(pos>0 ){
      instruc.mem1.assign(line,0,pos);
      line.erase(0,pos+1); //ja elimina o ;
      //*contador_mem = *contador_mem +1;

      if(line.length()>0){  //segundo operando;
        instruc.mem2 = line;
        //*contador_mem = *contador_mem +1;
      }
    }
    else if(line.length()>0){
      instruc.mem1 = line;
      line.erase(0,line.length());
      //*contador_mem = *contador_mem +1;
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


struct instr_format* create_instr(struct instr_format* Instr){
  
  int i;

  for(i=0;i<14;i++){
    Instr[i].code = i+1;
    Instr[i].tam = 2;
  }
  Instr[8].tam = 3;
  Instr[13].tam =1;

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

  //for(i=0;i<14;i++){
  //    cout<< Instr[i].name<<" "<<Instr[i].code<<" "<<Instr[i].tam<<endl;
  //}

  return Instr;
}


int procura_t_inst(struct instr_format *Instr, string instruc,int flag){

  int i=0;
  for(i=0;i<14;i++){
    if(Instr[i].name == instruc){
      switch(flag){
        case(1):
          return Instr[i].code;
        case(2):
          return Instr[i].tam;
      }

    }
  }

  return -1;
}

struct diret_format* create_diret(struct diret_format* Diret){
  
  Diret[0].rotulo = "SECTION";
  Diret[1].rotulo = "SPACE";
  Diret[2].rotulo = "CONST";
  Diret[3].rotulo = "EQU";
  Diret[4].rotulo = "IF";
  Diret[5].rotulo = "PLUBIC";
  Diret[6].rotulo = "EXTERN";
  Diret[7].rotulo = "BEGIN";
  Diret[8].rotulo = "END";

  Diret[0].args = 1;
  Diret[1].args = 0;  //**** MUDAAAAAAAAAAAAAAAAAAAAR*//
  Diret[2].args = 1;
  Diret[3].args = 1;
  Diret[4].args = 1;
  Diret[5].args = 0;
  Diret[6].args = 0;
  Diret[7].args = 0;
  Diret[8].args = 0;

  Diret[0].tam = 0;
  Diret[1].tam = 1;   
  Diret[2].tam = 1;
  Diret[3].tam = 0;
  Diret[4].tam = 0;
  Diret[5].tam = 0;
  Diret[6].tam = 0;
  Diret[7].tam = 0;
  Diret[8].tam = 0;
 


  return Diret;
}

int procura_tdir(diret_format *Diret,string diretiva,int flag){

  int i=0;

  for(i=0;i<9;i++){
    if(Diret[i].rotulo == diretiva){
      return Diret[i].tam;
    }
  }

  return -100;

}

int procura_ts(st_tab_simb* tabela_simb, string rotulo,int contador_ts){

  int i;
  for(i=0;i<contador_ts;i++){
    if(tabela_simb[i].rotulo == rotulo){
      return 1;
    }
  }
  return 0;
}




int executa_diretiva(string diretiva,string operando){

  int tam = 0;
  if(diretiva == "CONST"){
    
    return 1;
  }
  if(diretiva == "SPACE"){
    tam = 1;
    if(!operando.empty()){
      tam  = atoi(operando.c_str());
      
    }

  }
  return tam;

}
