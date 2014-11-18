#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include "parser.h"
#include "string.h"
#include "err.h"
#include "stack.h"

TStack *stackPSA;


int PrecedenceTABLE[20][20];



///Funckia ktora spravy precedencnu analyzu vyrazu alebo podmienky
int PrecedenceSA(LEX_STRUCT *LEX_STRUCTPTR,int *type)
{
if(((stackPSA=stack_init())==NULL))
    return E_INTERNAL;

int token=getnextToken(LEX_STRUCTPTR);

printf("TOKEN %i\n",token);



stack_free(stackPSA);
  return SUCCESS;

}
