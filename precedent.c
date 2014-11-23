#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scaner.h"
#include "parser.h"
#include "string.h"
#include "precedent.h"
#include "err.h"
#include "ial.h"
#include "stack.h"
#define EXPRESION 100

int PrecedenceSA(LEX_STRUCT*,int,THash_table*,THash_table*);

TStack *stackPSA;  /// stack pre tokeny
TStack *stackSEM;  /// stack pre pravidla
TStack *Helper;  ///na expresion
int term ;
int TOP_Stack;
int TOP_Stdin;
int concateT;
int PSA_Stalker;

int PrecedenceTABLE[13][13];

static int rule0[3]={EXPRESION,PSA_PLUS,EXPRESION};/// pravidlo E->E+E
static int rule1[3]={EXPRESION,PSA_MINUS,EXPRESION};///pravidlo E->E-E
static int rule2[3]={EXPRESION,PSA_MULT,EXPRESION};///pravidlo E->E*E
static int rule3[3]={EXPRESION,PSA_DIVIDE,EXPRESION};/// pravidlo E->E/E
static int rule4[3]={PSA_RIGHT,EXPRESION,PSA_LEFT};///pravidlo E->(E)
static int rule5[3]={PSA_ID,0,0};///pravidlo E->i
static int rule6[3]={EXPRESION,PSA_LESS,EXPRESION};///E->E < E
static int rule7[3]={EXPRESION,PSA_GREATER,EXPRESION};///E->E>E
static int rule8[3]={EXPRESION,PSA_LESSEQ,EXPRESION}; ///E->E<=E
static int rule9[3]={EXPRESION,PSA_GREATEREQ,EXPRESION};///E->E>=E
static int rule10[3]={EXPRESION,PSA_NOTEQUAL,EXPRESION};///E->E<>E
static int *rules[12]={rule0,rule1,rule2,rule3,rule4,rule5,rule6,rule7,rule8,rule9,rule10};
static int scan[3];///bude sa naplnat hodnotami zo zasobnika a nasledne porovnavat s pravidlami

///Funckia nam porovna so vsetkymi pravidlami ak najde
int Comparerule(int *scan)
{
int i=0;
while(i<11)
{
    if(strcmp(rules[i],scan)==0)
        return 0;
        i++;
}
return 1;
}
int checklex(int token)
{
 return ((token==E_LEXICAL) ? ERRORRET(token) : E_LEXICAL);
}


void initPrecedenceTABLE()
{
PrecedenceTABLE[PSA_PLUS][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_PLUS][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_PLUS][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_PLUS][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_PLUS][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_PLUS][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_MINUS][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_MINUS][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_MINUS][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_MINUS][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_MINUS][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_MINUS][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_MULT][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_MULT]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_DIVIDE]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_MULT][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_MULT][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_MULT][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_DIVIDE][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_MULT]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_DIVIDE]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_DIVIDE][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_DIVIDE][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_DIVIDE][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_ID][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_MULT]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_DIVIDE]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_ID]=0;
PrecedenceTABLE[PSA_ID][PSA_LEFT]=0;
PrecedenceTABLE[PSA_ID][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_ID][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_LEFT][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_RIGHT]=PT_EQUAL;
PrecedenceTABLE[PSA_LEFT][PSA_LESS]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_GREATER]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_LESSEQ]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_GREATEREQ]=PT_LESS;
PrecedenceTABLE[PSA_LEFT][PSA_DOLAR]=NULL;
PrecedenceTABLE[PSA_LEFT][PSA_NOTEQUAL]=PT_LESS;

PrecedenceTABLE[PSA_RIGHT][PSA_PLUS]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_MINUS]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_MULT]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_DIVIDE]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_ID]=0;
PrecedenceTABLE[PSA_RIGHT][PSA_LEFT]=0;
PrecedenceTABLE[PSA_RIGHT][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_LESS]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_GREATER]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_LESSEQ]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_GREATEREQ]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_RIGHT][PSA_NOTEQUAL]=PT_GREATER;

PrecedenceTABLE[PSA_LESS][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_LESS][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_LESS][PSA_LESS]=0;
PrecedenceTABLE[PSA_LESS][PSA_GREATER]=0;
PrecedenceTABLE[PSA_LESS][PSA_LESSEQ]=0;
PrecedenceTABLE[PSA_LESS][PSA_GREATEREQ]=0;
PrecedenceTABLE[PSA_LESS][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_LESS][PSA_NOTEQUAL]=0;

PrecedenceTABLE[PSA_GREATER][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_GREATER][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_GREATER][PSA_LESS]=0;
PrecedenceTABLE[PSA_GREATER][PSA_GREATER]=0;
PrecedenceTABLE[PSA_GREATER][PSA_LESSEQ]=0;
PrecedenceTABLE[PSA_GREATER][PSA_GREATEREQ]=0;
PrecedenceTABLE[PSA_GREATER][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_GREATER][PSA_NOTEQUAL]=0;

PrecedenceTABLE[PSA_GREATEREQ][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_GREATEREQ][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_GREATEREQ][PSA_LESS]=0;
PrecedenceTABLE[PSA_GREATEREQ][PSA_GREATER]=0;
PrecedenceTABLE[PSA_GREATEREQ][PSA_LESSEQ]=0;
PrecedenceTABLE[PSA_GREATEREQ][PSA_GREATEREQ]=0;
PrecedenceTABLE[PSA_GREATEREQ][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_GREATEREQ][PSA_NOTEQUAL]=0;

PrecedenceTABLE[PSA_LESSEQ][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_LESSEQ][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_LESSEQ][PSA_LESS]=0;
PrecedenceTABLE[PSA_LESSEQ][PSA_GREATER]=0;
PrecedenceTABLE[PSA_LESSEQ][PSA_LESSEQ]=0;
PrecedenceTABLE[PSA_LESSEQ][PSA_GREATEREQ]=0;
PrecedenceTABLE[PSA_LESSEQ][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_LESSEQ][PSA_NOTEQUAL]=0;

PrecedenceTABLE[PSA_DOLAR][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_RIGHT]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_LESS]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_GREATER]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_LESSEQ]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_GREATEREQ]=PT_LESS;
PrecedenceTABLE[PSA_DOLAR][PSA_DOLAR]=PT_OK;
PrecedenceTABLE[PSA_DOLAR][PSA_NOTEQUAL]=PT_LESS;

PrecedenceTABLE[PSA_NOTEQUAL][PSA_PLUS]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_MINUS]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_MULT]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_DIVIDE]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_ID]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_LEFT]=PT_LESS;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_RIGHT]=PT_GREATER;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_LESS]=0;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_GREATER]=0;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_LESSEQ]=0;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_GREATEREQ]=0;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_DOLAR]=PT_GREATER;
PrecedenceTABLE[PSA_NOTEQUAL][PSA_NOTEQUAL]=0;

}
int CheckEND(int end,int type)
{
 switch (end)
 {
   case THEN:
       {
    if(type==IF)
       return 0;
       }
   case DO:
       {
        if(type==WHILE)
        return 0;
       }

   case BODKOCIARKA:
       {


        if(type==ID)
		{
			ID_ENABLE=1;
			return 0;
		}
	case END:
	{
		if(type==ID)
		{
            ID_ENABLE = 2;
			return 0;
		}
	}


       }
 }
       return ERRORRET(end);
}
///Funkcia dekoduje jednotlive tokeny na indexy do PSA tabulky
int decodeSA(int token)
{
switch(token)
{
case DO:
    return PSA_DOLAR;
case BODKOCIARKA:
    ID_ENABLE = 1;
    return PSA_DOLAR;
case END:
    ID_ENABLE = 2;
    return PSA_DOLAR;
case THEN:
   return PSA_DOLAR;
case CONST:
    return PSA_ID;
case REALo:
    return PSA_ID;
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
case NOTEQUAL:
    return PSA_NOTEQUAL;
default:
    return ERRORRET(token);
}
}


int reduce(LEX_STRUCT *LEX_STRUCTPTR)
{
	int i = 0;




	while(term != ZARAZKA && term != PSA_DOLAR)
	{
        stack_top(stackPSA, &term);
		scan[i] = term;
		stack_pop(stackPSA);
		stack_top(stackPSA, &term);
		if(i == 2)
			break;
		i++;
	}
		if(term==ZARAZKA)
		stack_pop(stackPSA);
        stack_top(stackPSA, &term);
		stack_push(stackPSA, EXPRESION);



	if(i < 2)
	{
		scan[1] = 0;
		scan[2] = 0;
	}



	if(Comparerule(scan) != 0){

		return ERRORRET(TOP_Stdin);
	}

	PSA_Stalker = PrecedenceTABLE[term][decodeSA(TOP_Stdin)];



	if(PSA_Stalker == NULL)
		exit(E_SYNTAX);
	else if(PSA_Stalker == PT_GREATER)
		return reduce(LEX_STRUCTPTR);
	else if(PSA_Stalker == PT_LESS)
        return;
    else if (PSA_Stalker==PT_OK)
        return SUCCESS;
}



int PrecedentAnal(LEX_STRUCT *LEX_STRUCTPTR,int type)
{

if(TOP_Stdin==PSA_DOLAR && decodeSA(TOP_Stack)==PSA_DOLAR )
{
    CheckEND(TOP_Stdin,type);
    return 0;
}


PSA_Stalker= PrecedenceTABLE[TOP_Stack][decodeSA(TOP_Stdin)];


    if(PSA_Stalker!=NULL)
    {
     if(PSA_Stalker==PT_LESS)
     {
         stack_push(stackPSA,ZARAZKA);
         stack_push(stackPSA,decodeSA(TOP_Stdin));
         stack_top(stackPSA,&TOP_Stack);
         TOP_Stdin=getnextToken(LEX_STRUCTPTR);
        checklex(TOP_Stdin);
                                                        ///
        return PrecedentAnal(LEX_STRUCTPTR,type);
     }
     else if (PSA_Stalker==PT_GREATER)
     {

            term= TOP_Stack;
           if((reduce(LEX_STRUCTPTR))==SUCCESS)
            return 0;
           stack_push(stackPSA,decodeSA(TOP_Stdin));
           stack_top(stackPSA,&TOP_Stack);
           TOP_Stdin=getnextToken(LEX_STRUCTPTR);
           checklex(TOP_Stdin);

           return PrecedentAnal(LEX_STRUCTPTR,type);
     }
    }else


    return ERRORRET(TOP_Stdin);


}

///Funckia ktora spravy precedencnu analyzu vyrazu alebo podmienky
int PrecedenceSA(LEX_STRUCT *LEX_STRUCTPTR,int type,THash_table *GlobalnaTAB,THash_table*LokalnaTAB)
{
if(((stackPSA=stack_init())==NULL))
    exit(E_INTERNAL);
stack_push(stackPSA,PSA_DOLAR);///inicializacia vrcholu na dolar
stack_top(stackPSA,&TOP_Stack);
initPrecedenceTABLE();

   TOP_Stdin=getnextToken(LEX_STRUCTPTR);
   checklex(TOP_Stdin);
    if(TOP_Stdin!=ID && TOP_Stdin!=LEFT_ROUND && TOP_Stdin!=CONST && TOP_Stdin!=CONST_STRING)
        return ERRORRET(TOP_Stdin);


///Bude sa jednat o kokatenaciu retazcov  ++ dodat ked ID je CONST_STRING tak nech tiez konkatenuje +semanticku kontrolu
if(TOP_Stdin==CONST_STRING && type==ID)
{
    concate(LEX_STRUCTPTR,type);

}else
{
PrecedentAnal(LEX_STRUCTPTR,type);
}
stack_free(stackPSA);
  return SUCCESS;
}



///FUNKCIA sa stara o konkatenaciu 2 retazcov ID:='PR'+'D'
int concate(LEX_STRUCT *LEX_STRUCTPTR,int type)
{
    concateT=getnextToken(LEX_STRUCTPTR);


    if(concateT == PLUS)
    {
       concateT=getnextToken(LEX_STRUCTPTR);

       if(concateT==ID)
       {
           return concate(LEX_STRUCTPTR,type);
       }
       else if(concateT==CONST_STRING)
       {
           return concate(LEX_STRUCTPTR,type);

       }
            else return ERRORRET(concateT);

    }else if(decodeSA(concateT)==PSA_DOLAR)
    {
        CheckEND(concateT,type);
        return 0;
    }
    else
        return ERRORRET(concateT);
}
























