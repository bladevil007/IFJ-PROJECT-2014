
#ifndef STRING_H
#define STRING_H
#define STRLEN 8

typedef struct {
int value;
char *str;
int length;
int mallocsize;
}LEX_STRUCT;


int Init_str(LEX_STRUCT *LEX_STRUCTPTR);

void Free_str(LEX_STRUCT *LEX_STRUCTPTR);

void strClear(LEX_STRUCT *LEX_STRUCTPTR);

int AddChar_str(LEX_STRUCT *LEX_STRUCTPTR, char c);

int CmpConst_str(LEX_STRUCT *LEX_STRUCTPTR, char* CONST_STR);
#endif


