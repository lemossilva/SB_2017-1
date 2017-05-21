// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>

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
int procura_ts(vector<st_tab_simb>, string);

/* recebe diretiva, retorna seu tamanho alocado*/
int executa_diretiva(string,string);

void mostra_instrucao(string,string,string,string);

int main (int argv, char** argc) {

  string line,rotulo,op,mem1,mem2;
  int contador_mem =0, tam=-1,acha_diret=-1,existe_rotul=1,tamanho_diret=0,conta_linha=1;


  struct sep_instr instruc;
  //struct st_tab_simb *tabela_simb = NULL,*tab_temp=NULL;
  vector<st_tab_simb> tabela_simb;
  vector<st_tab_simb>::iterator tabela_simb_it;
  st_tab_simb simbolo1;

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


  /* Enquanto arquivo não chegar ao fim, pegue uma linha*/
  /********************************/
  while ( getline (myfile,line) ){
    
    /*** FUNCAO QUE SEPARA ELEMENTOS LINHA **/
    instruc = separa_linha(line);

    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;

    //mostra_instrucao(rotulo,op,mem1,mem2);

    
    /** VERIFICA SE EXISTE ROTULO **/
    if(!rotulo.empty()){
      /***procura por rotulo na TS**/
      existe_rotul = procura_ts(tabela_simb,rotulo);

      if(existe_rotul>0){
        cout<<"error (linha "<<conta_linha<<"):"<<rotulo<<" rotulo redefinido"<<endl;
        exit(1);
      }
      else{
        simbolo1.rotulo = rotulo;
        simbolo1.ender = contador_mem;
        tabela_simb.push_back(simbolo1);    //insere rotulo e posição  na tabela de simbolos
        
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
        cout<<"error lexico(linha: "<<conta_linha<<"): "<<op<<" operacao/diretiva invalida"<<endl; 
        exit(1);
      }
      
    }

    op.clear();
    mem1.clear();
    rotulo.clear();
    mem2.clear();

    conta_linha++;
  }

  

  /** MOSTRAR TABELA DE SIMBOLOS **/
  //int i;
  fp_tab_simb<<"TABLE USE"<<endl;

  for(tabela_simb_it = tabela_simb.begin(); tabela_simb_it!=tabela_simb.end(); tabela_simb_it++){
      simbolo1 = *tabela_simb_it;
      cout<<simbolo1.rotulo<<" "<<simbolo1.ender<<endl;
      fp_tab_simb<<simbolo1.rotulo<<" "<<simbolo1.ender<<endl;
    }

  myfile.clear();
  myfile.seekg(0, ios::beg);


  /******* SEGUNDA PASSAGEM ***********/
  /***********************************/

  int *realocation = (int*)calloc(contador_mem,sizeof(int));
  int *code_to_file = (int*)calloc(contador_mem,sizeof(int));

  contador_mem = 0;

  int contador_mem_old = 0;
  int inst_code;


  conta_linha=0;
  while(getline(myfile,line)){

    /* SEPARA ELEMENTOS DA LINHA */
    instruc = separa_linha(line);
    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;


    /** Existe operando e ele não é um numero**/
    if(!mem1.empty() && (atoi(mem1.c_str())==0)){
      //*procurar rotulo na tabela */
      existe_rotul = procura_ts(tabela_simb,mem1);
      //* Caso não seja encontrado */
      if(existe_rotul==-1){
          cout<<"error (linha "<<conta_linha<<"):rotulo "<<mem1<<" não definido"<<endl;
          exit(1);
      }
    }
    if(!mem2.empty() && (atoi(mem2.c_str())==0)){
      //*procurar rotulo na tabela */
      existe_rotul = procura_ts(tabela_simb,mem2);
      //* Caso não seja encontrado */
      if(existe_rotul==-1){
          cout<<"error (linha "<<conta_linha<<"):rotulo "<<mem2<<" não definido"<<endl;
          exit(1);
      }
    }

    //***** procura operação na tabela de instrucoes ***/
    tam = procura_t_inst(Instr,op,2);
    if(tam>0){
      contador_mem = contador_mem+tam;   // ****  incrementa contador de posição com tamanho da instrução
      //numero de operandos bate?
      if(tam == 1 + (!mem1.empty()) + (!mem2.empty())){
        inst_code = procura_t_inst(Instr,op,1);
        realocation[contador_mem_old] = 0;
        code_to_file[contador_mem_old] = inst_code;
        contador_mem_old++;

        //*codigo operando
        if(!mem1.empty()){
          realocation[contador_mem_old] = 1;
          existe_rotul = procura_ts(tabela_simb,mem1);
          code_to_file[contador_mem_old] = existe_rotul;
          contador_mem_old++;
        }
      }
    }else{
      ///**** EH UMA DIRETIVA
      acha_diret = procura_tdir(Diret,op,2);
      if(acha_diret>=0){ ///ACHOOOU
        //cout<<op<<endl;
        realocation[contador_mem] = 0;
        code_to_file[contador_mem] = -1;
        if(op =="CONST"){
          code_to_file[contador_mem] = atoi(mem1.c_str());
        }
        contador_mem++;
      }
      
    }



    


    conta_linha++;
  }

  int i=0;
  cout<<"realocation: "<<endl;
  for(;i<=contador_mem;i++){
    cout<<""<<realocation[i];
  }
  cout<<endl;
  cout<<"codes: "<<endl;
  for(i=0;i<=contador_mem;i++){
    cout<<" "<<code_to_file[i];
  }
  cout<<endl;

  myfile.close();
  fp_tab_simb.close();

  free(Instr);
  free(Diret);


  return 0;
}




struct sep_instr separa_linha(string line){

  struct sep_instr instruc;

  int pos;


   pos = (int)line.find(":");
    if(pos > 0){
      instruc.rotulo.assign(line,0,pos);
      line.erase(0,pos+1); //eliminar também o espaço após ':'
    }

    /**PROCURA OPERAÇÃO**/
    pos = (int)line.find(" "); //espaço para pegar codigo de instrução
    if(pos>0){
      instruc.op.assign(line,0,pos);  // pega codigo de instrução
      line.erase(0,pos+1); //+1 para eliminar espaço
    }else{ //if(op.empty()){
      /*caso do STOP e SPACE*/
      instruc.op=line;
      line.erase(0,line.length());
      //return caso faça função 
    }

    /** PRIMEIRO OPERANDO **/
    pos = (int)line.find(",");
    if(pos>0 ){
      instruc.mem1.assign(line,0,pos);
      line.erase(0,pos+1); //ja elimina o ;
      if(line.length()>0){  //segundo operando;
        instruc.mem2 = line;
      }
    }
    else if(line.length()>0){
      instruc.mem1 = line;
      line.erase(0,line.length());
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

int procura_ts(vector<st_tab_simb> tabela_simb, string rotulo){

  st_tab_simb simbolo1;
  int existe_rotul =-1;
  vector<st_tab_simb>::iterator tabela_simb_it;
      for(tabela_simb_it = tabela_simb.begin(); tabela_simb_it!=tabela_simb.end(); tabela_simb_it++){
        simbolo1 = *tabela_simb_it;
        if(rotulo == simbolo1.rotulo){
          existe_rotul = simbolo1.ender;
        }
      }
  return existe_rotul;
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
