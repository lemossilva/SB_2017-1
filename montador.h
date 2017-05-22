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

struct sep_instr separa_linha(string, int);
struct instr_format* create_instr(struct instr_format*);
struct diret_format* create_diret(struct diret_format*);
int procura_t_inst(struct instr_format*, string,int);
int procura_tdir(diret_format*,string,int);
int procura_ts(vector<st_tab_simb>, string, int*);
int executa_diretiva(string,string);
void mostra_instrucao(string,string,string,string);

#endif
