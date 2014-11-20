#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include "parser.h"
#include "string.h"
#include "precedent.h"
#include "err.h"
#include "stack.h"
#define EXPRESION 100

TStack *stackPSA;


int PrecedenceTABLE[12][12];


static int rule0[3]={EXPRESION,PSA_PLUS,EXPRESION};

///Funkcia dekoduje jednotlive tokeny na indexy do PSA tabulky
int decodeSA(int token)
{
switch(token)
{
case PLUS:
    return PSA_PLUS;
case MINUS:
    return PSA_MINUS;
case MULTIPLY:
    return PSA_MULT;
case DIVIDE:
    return PSA_DIVIDE;
case ID:
    return PSA_ID;
case LEFT_ROUND:
    return PSA_LEFT;
case RIGHT_ROUND:
    return PSA_RIGHT;
case LESS:
    return PSA_LESS;
case GREATER:
    return PSA_GREATER;
case LESSEQUAL:
    return PSA_LESSEQ;
case GREATEREQUAL:
    return PSA_GREATEREQ;
default:
    return ERRORRET(token);
}
}

///Funckia ktora spravy precedencnu analyzu vyrazu alebo podmienky
int PrecedenceSA(LEX_STRUCT *LEX_STRUCTPTR,int type)
{
    ///PRIDAT DO FUNKCIE
    PrecedenceTABLE[PSA_DOLAR][PSA_ID]=PT_LESS;

if(((stackPSA=stack_init())==NULL))
    exit(E_INTERNAL);

stack_push(stackPSA,'$');///inicializujeme koniec zasobniku na dolar
int Top_Stack=PSA_DOLAR;
int Top_Stdin;

int token=getnextToken(LEX_STRUCTPTR);
if(token!=LEFT_ROUND && token!=ID)
    return ERRORRET(token);


///Vsetko ok ulozime si token do TOP_stdin
Top_Stdin=token;
if(token==ID){
    stack_push(stackPSA,PSA_ID);///Pushneme na zasobnik prvy token bud ( alebo id
    stack_top(stackPSA,&Top_Stack);  ///ulozime novy vrchol zasobnika


}








stack_free(stackPSA);
  return SUCCESS;
}
