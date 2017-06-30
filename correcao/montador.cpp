#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>

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
      int ender,rot_extern;
} st_tab_simb;

typedef struct diret_format{
      string rotulo;
      int args,tam;

} diret_format;

typedef struct instr_format{
      string name;
      int code,tam;

} instr_format;

// recebe linha e ponteiro de error, separa 4 tolkens: rotulo, instrucao, operando1, operando2
struct sep_instr separa_linha(string, int, int*); // ponteiro de error

//struct instr_format* create_instr(struct instr_format*);

vector<instr_format> create_instr2(vector<instr_format> );

//struct diret_format* create_diret(struct diret_format*);
vector<diret_format> create_diret2(vector<diret_format>);


//int procura_t_inst(struct instr_format*, string,int);
int procura_t_inst2(vector<instr_format>, string,int);


//verifica se a o formato dos tolkens da instrução são validos
//int checa_tolken(string,string,string,string, struct instr_format*, struct diret_format*);
int checa_tolken2(string,string,string,string, vector<instr_format>,vector<diret_format>);
//verifica alguns erros sintaticos, outros podem estar na main
//checa_alguns_sintaticos(rotulo,op,mem1,mem2,Instr,Diret)
int checa_alguns_sintaticos(string,string,string);

//int  procura_tdir(diret_format*,string,int);
//int  procura_tdir2(diret_format*,string);
int  procura_tdir2(vector<diret_format>,string);

/** função que procura rotulo na tabela de simbolos, recebe tabela,rotulo e tamanho da tabela */
int procura_ts(vector<st_tab_simb>, string, int*);

/* recebe diretiva, retorna seu tamanho alocado*/
int executa_diretiva(string,string);

void mostra_instrucao(string,string,string,string);

int chama_montador(string,string);

int chama_montador(string nome_entrada,string nome_saida){

  string line,rotulo,op,mem1,mem2,mem_array,temp_mem;
  int contador_mem =0, tam=-1,acha_diret=-1,existe_rotul=1,tamanho_diret=0,conta_linha=1;

  int set_BEGIN =0, set_END =0, set_TEXT =0, set_STOP=0;//,set_DATA;  // checar se tem BEGIN, END e SECTION TEXT
  int *flag_EXTERN = (int*)calloc(1,sizeof(int)), compensa_array=0,pos,compensa_array2 =0,temp_memi=0;
  int *flag_ERROR = (int*)calloc(1,sizeof(int));

  stringstream converte_hex;
  ostringstream convert; 
  

  struct sep_instr instruc;
  //struct st_tab_simb *tabela_simb = NULL,*tab_temp=NULL;
  vector<st_tab_simb> tabela_simb,tabela_def, tabela_uso;
  vector<st_tab_simb>::iterator tabela_simb_it,tabela_def_it,tabela_uso_it;
  st_tab_simb simbolo1;

  //struct instr_format *Instr =NULL; //tabela de instruções
  vector<instr_format> Instr2;
  vector<instr_format>::iterator Instr2_it;

  vector<diret_format> Diret2;
  vector<diret_format>::iterator Diret2_it;
  //struct instr_format *Instr = (instr_format*)calloc(14,sizeof(instr_format));
  //struct diret_format *Diret = (diret_format*)calloc(9,sizeof(diret_format));

  /*if(Instr ==NULL ||Diret ==NULL){
  
    cout<<"falha na alocacao"<<endl;
    exit(1);
  }*/

  Instr2 = create_instr2(Instr2);  //cria tabela de instruções
  Diret2 = create_diret2(Diret2);  //cria tabela de instruções




    /*** ARQUIVOS   ***/
  ifstream myfile (nome_entrada.c_str());  //abre arquivo fonte
  //nome_saida = argc[2];
  nome_saida = nome_saida + ".o";
  ofstream fp_fileout (nome_saida.c_str());        //abre tabela de simbolos

  //verifica se arquivos foram abertos
  if (!fp_fileout.is_open() || ! myfile.is_open()){  
    cout<<"arquivo não abriu"<< endl;
    return 1;
  }

  /****         PRIMEIRA PASSAGEM *******/

  /* Enquanto arquivo não chegar ao fim, pegue uma linha*/
  /********************************/
  while ( getline (myfile,line) ){
    
    /*** FUNCAO QUE SEPARA ELEMENTOS LINHA **/
    instruc = separa_linha(line,conta_linha,flag_ERROR);
    if(*flag_ERROR == 1){
      cout<<"linha "<<conta_linha<<" :\""<<line<<"\""<<endl;
      exit(1);
    }


    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;

     if(checa_tolken2(rotulo,op,mem1,mem2,Instr2,Diret2)){
      cout<<"linha "<<conta_linha<<" :\""<<line<<"\""<<endl;
      exit(1);
     }

  
    if(checa_alguns_sintaticos(op,mem1,mem2)){
      cout<<"linha "<<conta_linha<<" :\""<<line<<"\""<<endl;
      exit(1);
    }
    //mostra_instrucao(rotulo,op,mem1,mem2);

    /** VERIFICA SE EXISTE ROTULO **/
    if(!rotulo.empty()){
      /***procura por rotulo na TS**/
      existe_rotul = procura_ts(tabela_simb,rotulo,flag_EXTERN);


      if(existe_rotul>0){
        cout<<"error semantico, rotulo \":"<<rotulo<<"\" redefinido"<<endl;
        cout<<"linha "<<conta_linha<<": "<<line<<endl;
        exit(1);
      }
      else{
        simbolo1.rotulo = rotulo;
        simbolo1.ender = contador_mem;
        simbolo1.rot_extern = 0;
        if(op=="EXTERN"){
          simbolo1.rot_extern =1;
        }
        tabela_simb.push_back(simbolo1);    //insere rotulo e posição  na tabela de simbolos
        
      }
    }

    //*procura por intrução na tabela de instruções **/
    tam = procura_t_inst2(Instr2,op,2);
    //*** achou instrução na tabela**/
    if(tam>0){
      contador_mem = contador_mem+tam;   // ****  incrementa contador de posição com tamanho da instrução
    }else{
      //**** procura na tabela de diretivas**//
      acha_diret = procura_tdir2(Diret2,op);
      if(acha_diret>=0){ ///ACHOOOU
        //*executa diretiva*//
        if(op =="BEGIN"){
          //cout<<"existe BEGIN"<<endl;
          set_BEGIN =1;
        }else if(op == "END"){
          //cout<<"existe END"<<endl;
          set_END = 1;
        }else if(op == "SECTION" && mem1 == "TEXT"){
          //cout<<"there is a TEXT SECTION"<<endl;
          set_TEXT = 1;
        }else if(op == "SECTION" && mem1 == "DATA"){
          if(set_TEXT==0){
            cout<<"erro semantico: SECTION DATA antes de SECTION TEXT:"<<endl;
            cout<<"linha "<<conta_linha<<": "<<line<<endl;
            exit(1);
          }
        }
        else if(op == "PUBLIC"){
          simbolo1.rotulo = mem1;
          simbolo1.ender = -1;
          simbolo1.rot_extern = -1;
          tabela_def.push_back(simbolo1);
        }

        tamanho_diret = executa_diretiva(op,mem1);
        contador_mem= contador_mem + tamanho_diret;
      }
      else{
        cout<<"error lexico : \""<<op<<"\" operacao/diretiva invalida"<<endl; 
        cout<<"linha "<<conta_linha<<" :"<<line<<endl;
        exit(1);
      }
      
    }
    if(op == "STOP"){
      set_STOP =1;
    }

    op.clear();
    mem1.clear();
    rotulo.clear();
    mem2.clear();

    conta_linha++;
    
  }

  
  if(set_BEGIN && !set_END){
    cout<<"erro semantico: falta de 'END'"<<endl;
    exit(1);
  }else if(set_END && !set_BEGIN){
    cout<<"erro semantico: falta de 'BEGIN'"<<endl;
    exit(1);
  }
  if(!set_TEXT){
    cout<<"erro semantico: falta de SECTION TEXT"<<endl;
    exit(1);
  }
  
  if(!set_STOP && !set_BEGIN && !set_END){
    cout<<"erro semantico: falta de STOP: "<<endl;
    exit(1);
  }

  

  //fp_fileout<<"\nDEFINITION TABLE"<<endl;

  for(tabela_def_it=tabela_def.begin(); tabela_def_it!=tabela_def.end() ; tabela_def_it++){
      tabela_def_it->ender = procura_ts(tabela_simb,simbolo1.rotulo,flag_EXTERN);
      simbolo1 = *tabela_def_it;
      tabela_def_it->ender = procura_ts(tabela_simb,simbolo1.rotulo,flag_EXTERN);
      
  }
  //cout<<"FIM PRIMEIRA PASSAGEM"<<endl;

  myfile.clear();
  myfile.seekg(0, ios::beg);

  
    /******* SEGUNDA PASSAGEM ***********/
    /***********************************/

  vector<int> realocation,code_to_file;
  vector<int>::iterator realocation_it, code_to_file_it;


  contador_mem = 0;

  //int contador_mem_old = 0;
  int inst_code;


  conta_linha=1;
  while(getline(myfile,line)){

    /* SEPARA ELEMENTOS DA LINHA */
    instruc = separa_linha(line,conta_linha,flag_ERROR);

    while(instruc.op == "SECTION" || instruc.op == "BEGIN"){
      // não gera codigo executavel
      // avaliar essas diretivas aqui
      getline(myfile,line);
      conta_linha++;
      instruc = separa_linha(line,conta_linha,flag_ERROR);
    }
    op = instruc.op;
    mem1 = instruc.mem1;
    rotulo = instruc.rotulo;
    mem2 = instruc.mem2;

    pos = (int)mem1.find("0x");
    if(pos == 0){
      //hexadecimal
      temp_mem.assign(mem1,2,mem1.length());
      temp_memi = atoi(temp_mem.c_str());
      converte_hex << temp_memi;
      converte_hex>> std::hex>> temp_memi;
      convert << temp_memi;
      mem1 = convert.str();
      //cout<<"convert: "<<mem1;
    }
    /** Existe operando e ele não é um numero**/
    if(!mem1.empty() && (atoi(mem1.c_str())==0)){

      //*** verifica se foi feita utilização de vetor *////
      pos = (int)mem1.find("+");
      compensa_array=0;
      if(pos>0){
        mem_array.assign(mem1,pos+1,mem1.length());
        compensa_array = atoi(mem_array.c_str());
        mem1.assign(mem1,0,pos);
      }

      //*procurar rotulo na tabela */
      existe_rotul = procura_ts(tabela_simb,mem1,flag_EXTERN);
      //* Caso não seja encontrado */
      if(existe_rotul==-1){
          cout<<"error semantico: rotulo \""<<mem1<<"\" não definido"<<endl;
          cout<<"linha "<<conta_linha<<" \""<<line<<"\""<<endl;
          exit(1);
      }
    }
    /** Segundo operando não é um numero**/

    if(!mem2.empty() && (atoi(mem2.c_str())==0)){

      //*** verifica se foi feita utilização de vetor *////
      pos = (int)mem2.find("+");
      compensa_array2=0;
      if(pos>0){
        mem_array.assign(mem2,pos+1,mem2.length());
        compensa_array2 = atoi(mem_array.c_str());
        mem2.assign(mem2,0,pos);
      }

      //*procurar rotulo na tabela */
      existe_rotul = procura_ts(tabela_simb,mem2,flag_EXTERN);
      //* Caso não seja encontrado */
      if(existe_rotul==-1){
          cout<<"error (linha "<<conta_linha<<"):rotulo "<<mem2<<" não definido"<<endl;
          exit(1);
      }
    }

    //***** procura operação na tabela de instrucoes ***/
    tam = procura_t_inst2(Instr2,op,2);
    if(tam>0){
      contador_mem = contador_mem+tam;   // ****  incrementa contador de posição com tamanho da instrução
      //numero de operandos bate?
      if(tam == 1 + (!mem1.empty()) + (!mem2.empty())){
        inst_code = procura_t_inst2(Instr2,op,1);
        realocation.push_back(0);
        code_to_file.push_back(inst_code);
        //realocation[contador_mem_old] = 0;
        //code_to_file[contador_mem_old] = inst_code;
        //contador_mem_old++;

        //*codigo operando
        if(!mem1.empty()){
          //realocation[contador_mem_old] = 1;
          
          existe_rotul = procura_ts(tabela_simb,mem1,flag_EXTERN);
          //code_to_file[contador_mem_old] = existe_rotul;
          if(*flag_EXTERN == 1){
            simbolo1.rotulo = mem1;
            simbolo1.ender = code_to_file.size();
            tabela_uso.push_back(simbolo1);
          }
          realocation.push_back(1);
          code_to_file.push_back(existe_rotul+compensa_array);
          //se o rotulo é externo
          //contador_mem_old++;
        }if(!mem2.empty()){

          existe_rotul = procura_ts(tabela_simb,mem2,flag_EXTERN);
          if(*flag_EXTERN == 1){
            simbolo1.rotulo = mem2;
            simbolo1.ender = code_to_file.size();
            tabela_uso.push_back(simbolo1);
          }
          realocation.push_back(1);
          code_to_file.push_back(existe_rotul+compensa_array2);
        }
      }else{
        cout<<"error: quantidade incorreta de operandos:"<<endl;
        cout<<"linha "<<conta_linha<<" : '"<<line<<"'"<<endl;
        exit(1);
      }

    }else if(op!= "END"){
      ///**** EH UMA DIRETIVA
      //cout<<"diretiva :"<<op<<endl;
      acha_diret = procura_tdir2(Diret2,op);
      if(acha_diret>=0){ ///ACHOOOU
        //realocation[contador_mem] = 0;
        //code_to_file[contador_mem] = -1;
        
        if(op =="CONST"){
          //code_to_file[contador_mem] = atoi(mem1.c_str());
          realocation.push_back(0);
          code_to_file.push_back(atoi(mem1.c_str()));
        }else if(op == "SPACE"){
          int space_tam = atoi(mem1.c_str());
          int cont=1;
          
          if(space_tam==0){
            code_to_file.push_back(0);
            realocation.push_back(0);
          }
          for(cont=1;cont<=space_tam;cont++){
            code_to_file.push_back(0);
            realocation.push_back(0);
          }
        }else{
          //code_to_file.push_back(0); // retirei por causa do PUBLIC, EXTERN

        }
        //contador_mem++;
      }
      
    }


    conta_linha++;
  }

  



  fp_fileout<<"TABLE USE"<<endl;
  for(tabela_uso_it=tabela_uso.begin(); tabela_uso_it!=tabela_uso.end(); tabela_uso_it++){
    simbolo1 = *tabela_uso_it;
    fp_fileout<<simbolo1.rotulo<<" "<<simbolo1.ender<<endl;
  }

  fp_fileout<<"TABLE DEFINITION"<<endl;
  for(tabela_def_it=tabela_def.begin(); tabela_def_it!=tabela_def.end() ; tabela_def_it++){
    simbolo1 = *tabela_def_it;
    
    fp_fileout<<simbolo1.rotulo<<" "<<simbolo1.ender<<endl;
}//fim segunda passagem argc

  fp_fileout<<"TABLE REALOCATION"<<endl;
  for(realocation_it = realocation.begin();realocation_it!=realocation.end();realocation_it++){
    fp_fileout<<*realocation_it;
  }
  fp_fileout<<"\nCODE"<<endl;
  for(code_to_file_it=code_to_file.begin(); code_to_file_it!=code_to_file.end(); code_to_file_it++){
    fp_fileout<<*code_to_file_it<<" ";
  }
    
  //}


  myfile.close();
  fp_fileout.close();

  //free(Instr);
  //free(Diret);
  free(flag_EXTERN);
  free(flag_ERROR);


  return 0;
}




struct sep_instr separa_linha(string line, int conta_linha,int* error){

  struct sep_instr instruc;

  int pos;
  *error =0;

   pos = (int)line.find(":");
    if(pos > 0){
      instruc.rotulo.assign(line,0,pos);
      line.erase(0,pos+1); //eliminar também o espaço após ':'
      if((int)line.find(":")>0){
        cout<<"erro sintatico, dois rotulos na mesma linha"<<endl;
        *error = 1;
        return instruc;
      }
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
        line.erase(0,instruc.mem2.length());
      }
    }
    else if(line.length()>0){
      pos = (int)line.find(" ");
      if(pos<0){
        instruc.mem1 = line;
        line.erase(0,pos);
      }else{
        instruc.mem1.assign(line,0,pos);
        line.erase(0,pos+1); //ja elimina o " "
        instruc.mem2.assign(line,0,pos);
        line.erase(0,pos);
      }
    }
    //*contador_mem = 2;
    if(!line.empty()){
      cout<<"error sintatico, (linha "<<conta_linha<<"): mais de 2 operandos"<<endl;
      *error = 1;
    }

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

vector<instr_format> create_instr2(vector<instr_format> Instr){

  struct instr_format Instr_p;

  Instr_p.code = 1;
  Instr_p.tam = 2;
  Instr_p.name = "ADD";
  Instr.push_back(Instr_p);
  Instr_p.code = 2;
  Instr_p.tam = 2;
  Instr_p.name = "SUB";
  Instr.push_back(Instr_p);
  Instr_p.code = 3;
  Instr_p.tam = 2;
  Instr_p.name = "MULT";
  Instr.push_back(Instr_p);
  Instr_p.code = 4;
  Instr_p.tam = 2;
  Instr_p.name = "DIV";
  Instr.push_back(Instr_p);
  Instr_p.code = 5;
  Instr_p.tam = 2;
  Instr_p.name = "JMP";
  Instr.push_back(Instr_p);
  Instr_p.code = 6;
  Instr_p.tam = 2;
  Instr_p.name = "JUMPN";
  Instr.push_back(Instr_p);
  Instr_p.code = 7;
  Instr_p.tam = 2;
  Instr_p.name = "JUMPP";
  Instr.push_back(Instr_p);
  Instr_p.code = 8;
  Instr_p.tam = 2;
  Instr_p.name = "JMPZ";
  Instr.push_back(Instr_p);
  Instr_p.code = 9;
  Instr_p.tam = 3;
  Instr_p.name = "COPY";
  Instr.push_back(Instr_p);
  Instr_p.code = 10;
  Instr_p.tam = 2;
  Instr_p.name = "LOAD";
  Instr.push_back(Instr_p);
  Instr_p.code = 11;
  Instr_p.tam = 2;
  Instr_p.name = "STORE";
  Instr.push_back(Instr_p);
  Instr_p.code = 12;
  Instr_p.tam = 2;
  Instr_p.name = "INPUT";
  Instr.push_back(Instr_p);
  Instr_p.code = 13;
  Instr_p.tam = 2;
  Instr_p.name = "OUTPUT";
  Instr.push_back(Instr_p);
  Instr_p.code = 14;
  Instr_p.tam = 1;
  Instr_p.name = "STOP";
  Instr.push_back(Instr_p);

  //cout<<"valor: "<< Instr[11].name;

  return Instr;
}




int procura_t_inst2(vector<instr_format> Instr, string instruc,int flag){

  vector<instr_format>::iterator Instr_it;
  int i=0,j=0;
    //cout<<"comecou"<<endl;
  for(Instr_it = Instr.begin();Instr_it!=Instr.end();Instr_it++ ){
    //cout<<Instr[j].name<<endl;
  //for(i=0;i<14;i++){
    if(Instr[j].name == instruc){
      switch(flag){
        case(1):
          return Instr[j].code;
        case(2):
          return Instr[j].tam;
      }

    }
    j++;
  }

  return -1;
}

struct diret_format* create_diret(struct diret_format* Diret){
  
  Diret[0].rotulo = "SECTION";
  Diret[1].rotulo = "SPACE";
  Diret[2].rotulo = "CONST";
  Diret[3].rotulo = "EQU";
  Diret[4].rotulo = "IF";
  Diret[5].rotulo = "PUBLIC";
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

vector<diret_format> create_diret2(vector<diret_format>Diret_r){

  diret_format Diret;

  Diret.rotulo = "SECTION";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "SPACE";
  Diret.args = 0;  //**** MUDAAAAAAAAAAAAAAAAAAAAR*//
  Diret.tam = 1;
  Diret_r.push_back(Diret);
  Diret.rotulo = "CONST";
  Diret.args = 1;
  Diret.tam = 1;
  Diret_r.push_back(Diret);
  Diret.rotulo = "EQU";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "IF";
  Diret.args = 1;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "PUBLIC";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "EXTERN";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "BEGIN";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);
  Diret.rotulo = "END";
  Diret.args = 0;
  Diret.tam = 0;
  Diret_r.push_back(Diret);

 


  return Diret_r;
}

// recebe tabela de diretivas, nome a ser procurado e flag nao usada

int procura_tdir2(vector<diret_format> Diret,string diretiva){

  int i=0;
  vector<diret_format>::iterator Diret_it;
  for(Diret_it = Diret.begin();Diret_it!=Diret.end();Diret_it++){
  //for(i=0;i<9;i++){
    if(Diret[i].rotulo == diretiva){
      return Diret[i].tam;
    }
    i++;
  }

  return -100;

}


int procura_ts(vector<st_tab_simb> tabela_simb, string rotulo,int* flag_EXTERN){

  
  st_tab_simb simbolo1;
  int existe_rotul =-1;
  vector<st_tab_simb>::iterator tabela_simb_it;
    for(tabela_simb_it = tabela_simb.begin(); tabela_simb_it!=tabela_simb.end(); tabela_simb_it++){
      simbolo1 = *tabela_simb_it;
      if(rotulo == simbolo1.rotulo){
        existe_rotul = simbolo1.ender;
        *flag_EXTERN = simbolo1.rot_extern;
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


int checa_tolken2(string rotulo,string op,string mem1,string mem2,vector<instr_format>Instr,vector<diret_format> Diret){

  vector<instr_format>::iterator Instr_it;
  int flag, teste=0;
  if(!rotulo.empty()){
    //* checa se rotulo possui nome de instrucao *//
    flag = procura_t_inst2(Instr,rotulo,1);
    if(flag>=0){
      cout<<"erro lexico, rotulo \""<<rotulo<<"\" possui nome reservado:"<<endl;
      return 1;
    }
    //* checa se rotulo possui nome de diretiva */
    flag = procura_tdir2(Diret,rotulo);
    if(flag>=0){
      cout<<"erro lexico, rotulo \""<<rotulo<<"\" possui nome reservado:"<<endl;
      return 1;
    }
    // verifica se rotulo é text ou data
    if(rotulo=="TEXT" || rotulo == "DATA"){
      cout<<"erro lexico, rotulo \""<<rotulo<<"\" possui nome reservado:"<<endl;
      return 1;
    } 

     //** CHECA SE TEM COMECA COM NUMERO**//
    teste = atoi(rotulo.c_str());
    if(teste!=0){
      cout<<"erro lexico, argumento \""<<rotulo<<"\" comeca com numero:"<<teste<<endl;
      return 1;
    }
    teste=(int)rotulo.find("0");
    if(teste==0){
      cout<<"erro lexico, argumento \""<<rotulo<<"\" comeca com numero:"<<teste<<endl;
      return 1;
    }

    if(rotulo.length()>50){
      cout<<"erro lexico, rotulo \""<<rotulo<<"\" maior que o limite de 50 caracteres:\n"<<endl;
      return 1;
    }


  }
  if(!op.empty()){

  }
  if(!mem1.empty()){
    //* checa se rotulo possui nome de instrucao *//
    flag = procura_t_inst2(Instr,mem1,1);
    if(flag>=0){
      cout<<"erro lexico, argumento \""<<mem1<<"\" possui nome reservado:"<<endl;
      return 1;
    }
    //* checa se rotulo possui nome de diretiva *//
    flag = procura_tdir2(Diret,mem1);
    if(flag>=0){
      cout<<"erro lexico, argumento \""<<mem1<<"\" possui nome reservado:"<<endl;
      return 1;
    }



  }
  if(!mem2.empty()){
    //* POSSUI NOME DE INSTRUCAO ???? **///
    flag = procura_t_inst2(Instr,mem2,1);
    if(flag>=0){
      cout<<"erro lexico, argumento \""<<mem2<<"\" possui nome reservado:"<<endl;
      return 1;
    }
    //* checa se rotulo possui nome de diretiva */
    flag = procura_tdir2(Diret,mem2);
    if(flag>=0){
      cout<<"erro lexico, argumento \""<<mem2<<"\" possui nome reservado:"<<endl;
      return 1;
    }

  }

  return 0;

}
//int checa_alguns_sintaticos(string rotulo,string op,string mem1,string mem2,struct instr_format* Instr, struct diret_format* Diret);
int checa_alguns_sintaticos(string op,string mem1,string mem2){

  int pos;

  pos = mem1.find(" ");
  if(pos>=0){
    cout<<"erro sintatico, mais de um operando \""<<mem1<<"\""<<endl;
    return 1;
  }
  pos = mem2.find(" ");
  if(pos>=0){
    cout<<"erro sintatico, mais de um operando \""<<mem2<<"\""<<endl;
    return 1;
  }
  if(op == "SECTION"){
    if(mem1 != "TEXT" && mem1 != "DATA"){
      cout<<"erro sintatico, seção invalida :x \""<<mem1<<"\""<<endl;
      return 1;
    }
    if(!mem2.empty()){
      cout<<"erro sintatico, mais de um argumento: \""<<mem2<<"\""<<endl;
      return 1;
    } 
  }
  if(op == "CONST" || op == "SPACE"){
    if(!mem2.empty()){
      cout<<"erro sintatico, mais de um argumento: \""<<mem2<<"\""<<endl;
      return 1;
    }
  }
  if(op == "BEGIN" || op == "END"){
    if(!mem1.empty()){
      cout<<"erro sintatico, quantidade invalida de argumentos: \""<<mem1<<"\""<<endl;
      return 1;
    }
  }

  return 0;
}
