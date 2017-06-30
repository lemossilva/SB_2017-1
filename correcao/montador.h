#ifndef MONTADOR
#define MONTADOR

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
//int  procura_tdir(diret_format*,string);
int  procura_tdir2(vector<diret_format>,string);
/** função que procura rotulo na tabela de simbolos, recebe tabela,rotulo e tamanho da tabela */
int procura_ts(vector<st_tab_simb>, string, int*);
/* recebe diretiva, retorna seu tamanho alocado*/
int executa_diretiva(string,string);
void mostra_instrucao(string,string,string,string);
int chama_montador(string,string);
#endif
