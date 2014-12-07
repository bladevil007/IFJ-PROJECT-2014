#ifndef STRING_H
#define STRING_H

#define STRLEN 8
#define ARR_INC 8


typedef struct
{
  int *pole;		// samotne pole
  int volny_index;		// prvni volny index v poli
  int velikost_pole;	//velikost alokovaneho pole (nasobky 20)
} Tpole_int;

Tpole_int *IntArrayInit(); //inicializuje pole
int IntArrayAddItem(Tpole_int *p, int i); //prida prvek do pole
void IntArrayFree(Tpole_int *p); //uvolni pole
void IntArrayStructFree(Tpole_int *p); //uvolni celou strukturu


typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int act_pos;      // aktualna pozicia
  int allocSize;	// velikost alokovane pameti
} inf_array;


typedef struct {
double value;
char *str;
int length;
int mallocsize;
}LEX_STRUCT;

int Init_str(LEX_STRUCT *LEX_STRUCTPTR);

void Free_str(LEX_STRUCT *LEX_STRUCTPTR);

void strClear(LEX_STRUCT *LEX_STRUCTPTR);

int AddChar_str(LEX_STRUCT *LEX_STRUCTPTR, char c);

int CmpConst_str(LEX_STRUCT *LEX_STRUCTPTR, char* CONST_STR);

//nekonecne pole
#define ARR_INC 8

int add_str(inf_array *a, char* strin);

void free_array(inf_array *a);

int init_array(inf_array *a);

int add_str_param(inf_array *, char* );

char *concatenate(const char *s1, const char *s2);
#endif


