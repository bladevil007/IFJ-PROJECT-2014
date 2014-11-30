int generate_inst(char*,float,float,int,int);



typedef struct {
char *a;
float b;
float c;
int specialcode;
int CODE;
}INSTape;



typedef struct{
    int length;
    int allocSize; // velikost alokovane pameti
    INSTape** pole; // misto pro dany retezec ukonceny znakem '\0'
} inf_pointer_array;

inf_pointer_array *InstructionTape;



int init_pointer_array(inf_pointer_array *a);
void free_pointer_array(inf_pointer_array *a);

int add_new_pointer(inf_pointer_array *a, INSTape* pointer);
int generate_inst(char *A,float B,float C,int CODE,int what);
int init_generate();
int foo(INSTape *INSTR);
int searchrecord(inf_pointer_array* beh_programu);

#define BEGIN_MAIN 200
#define END_MAIN 201
#define MOV 202
#define WRITESTRING 203
#define WRITEBOOL  204
#define WRITEINT 205
#define WRITEREAL 206
#define WRITEID 207
#define COPYSTRING 208
#define COPYID 209
#define SORTSTRING 210
#define SORTID 211
#define CONCATESTRING 212
#define CONCATEID 213
#define LENGTHID 214
#define ADD 215
#define ADDH 216
