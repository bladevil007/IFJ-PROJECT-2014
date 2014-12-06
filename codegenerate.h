



typedef struct {
char *a;
char *a2;
char *a3;
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
int generate_inst(char *,char *,float ,float ,int ,int,char * );
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
#define COPYSTRINGID_ 217
#define COPYSTRINGIDID 218
#define COPYSTRING_ID 219
#define COPYIDID_ 220
#define COPYID_ID 221
#define COPYIDIDID 226
#define FINDSTRSTR 222
#define FINDIDSTR 223
#define FINDSTRID 224
#define FINDIDID 225
#define JUMP 227
#define BEGINJUMP 228
#define ENDJUMP 229
#define SAVE 230
#define LOOP 231
#define LOOPJUMP 232
#define STARTFUN 233
#define ENDFUN 234
#define CALLFUN 235
#define VALUE 236
#define DECLARE 237

