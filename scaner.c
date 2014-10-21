/* **************************** scaner.c ************************************ */
/* Soubor:              scaner.c - Lexikalna analyza                          */
/* Kodovani:            UTF-8                                                 */
/* Datum:               11.2014                                               */
/* Predmet:             Formalni jazyky a prekladace (IFJ)                    */
/* Projekt:             Implementace interpretu jazyka IFJ14                  */
/* Varianta zadani:     a/2/II                                                */
/* Titul,Autori, login:         Ing. Natalya Loginova   xlogin00              */
/*                              Jindrich Dudek          xdudek04              */
/*                              Norbert Durcansky       xdurca01              */
/*                              Jan Jusko               xjusko00              */
/*                              Jiří Dostál             xdosta40              */
/* ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "parser.h"
#include "err.h"
#include "string.h"
#include "scaner.h"

FILE *source;
char *key[]={"begin","end","boolean","do",
"else","false","find","forward",
"function","if","integer","readln","real","sort",
"string","then","true","var","while","write"
};

/** \brief funkcia inicializuje zdrojovy subor
 *
 * \param  File pointer
 *
 */

void SourceInitialize(FILE *f)
{

source=f;

}

/** \brief funkcia vrati dalsi LEX_STRUCT , volana syntaktickou analyzou
 * \param  char pointer
 * \return LEX_STRUCT alebo lexikalna chyba
 */

int getnextToken (LEX_STRUCT *LEX_STRUCTPTR)    // parameter sa bude predavat ukazatel na strukturu LEX_STRUCT  viz string.h + ukazatel na pole
{
  LEX_STRUCTPTR->value=0;
  strClear(LEX_STRUCTPTR);

char c;   // premenna do ktorej si ukladame znak
int state=0;
int plusminus=0;
double value=0;
int u=1;
int plus_s=0; //
    while(1)
    {

c=getc(source);


///ODSTRANENIE KOMENTOV
if(c=='{' )
    state=KOMENT;
if ((state==KOMENT) && (c=='}'))
    state=0;
///********************************
if(state==REALo)
{
if((c >= '0') && (c <='9'))
{
value=(value*10)+(c-'0');
}

else if(value==0)
{
if(plusminus==0)
{
fseek(source,ftell(source)-1,SEEK_SET);
return E_LEXICAL;
}
fseek(source,ftell(source)-4,SEEK_SET);
return CONST;
}
else if((isspace(c)!=0) || c==EOF || c=='+'|| c=='-'|| c=='*'|| c=='/'|| c==';')
{

LEX_STRUCTPTR->value= LEX_STRUCTPTR->value*pow(10,value);
if(plus_s==1)
    LEX_STRUCTPTR->value=LEX_STRUCTPTR->value*(-1);
fseek(source,ftell(source)-1,SEEK_SET);
return CONST;
}
else
    return E_LEXICAL;
}

if(state==DOT)
{

if((c >= '0') && (c <='9')){
LEX_STRUCTPTR->value=(LEX_STRUCTPTR->value+((c-'0')*pow(10,-u)));
u++;
}
if((c < '0') || (c >'9'))
{
state=CONST;
}

}

///STAVY TVORENE JEDNOU LEXEMOU
if(state==0){
switch(c) {
case '<' :
    return LESS;
case '>' :
       return GREATER;
case '=' :
    return EQUAL;
case '.':
    return DOT;
case ':' :
     return DVOJBODKA;
case ';' :
     return BODKOCIARKA;
case '+' :
     return PLUS;
case '-' :
     return MINUS;
case '/' :
     return DIVIDE;
case '*' :
    return MULTIPLY;
case '(' :
    return LEFT_ROUND;
case ')' :
    return RIGHT_ROUND;
case 39 :
    return APOSTROF;
case '[' :
    return LEFT_HRANATA;
case ']' :
    return RIGHT_HRANATA;
}
}
///CISLA A KONSTANTY

if((c >= '0') && (c <='9')&&(state==0))
 state=CONST;

if((c >= '0') && (c <='9')&& (state==CONST)){
LEX_STRUCTPTR->value=(LEX_STRUCTPTR->value*10)+(c-'0');
}

if((c < '0') || (c >'9')){
if(state==CONST){
 if(isspace(c)!=0)
{
    return CONST;
 }

else if(c=='-')
{
state=REAL;
plus_s=1;
}
else if(c=='.')
state=DOT;


else if(c=='+')
state=REAL;


else if((c=='e') || (c=='E'))
state=REALo;

else if ((c==';')|| (c=='*') || (c=='/'))
 {

     fseek(source,ftell(source)-1,SEEK_SET);
     return CONST;
 }
 else
    if(c==EOF)
 {
return CONST;
 }
 else
    return E_LEXICAL;

}
}


if(state==REAL)
{
plusminus=1;
c=getc(source);
if((c=='e') || (c=='E'))
   state=REALo;
else
{
    fseek(source,ftell(source)-3,SEEK_SET);
     return CONST;
}
}
///*********************************************************** identifikatory a klucove slova

if((((c>=65)&&(c<=90))||  ((c>=97)&&(c<=122))||(c=='_'))&& (state==0))
{
state=STRING;
}
if(state==STRING)
{
if(((((c>=65)&&(c<=90))||  ((c>=97)&&(c<=122))||(c=='_'))) || ((c >= '0') && (c <='9')))

    AddChar_str(LEX_STRUCTPTR,c);

    else
    {
    for(int i=0;i<20;i++)
    {

     if((CmpConst_str(LEX_STRUCTPTR,key[i]))==0)
     {
         fseek(source,ftell(source)-1,SEEK_SET);
         switch(i)
         {
             case 0:return BEGIN;
             case 1:return END;
             case 2:return BOOLEAN;
             case 3:return DO;
             case 4:return ELSE;
             case 5:return FALSE;
             case 6:return FIND;
             case 7:return FORWARD;
             case 8:return FUNCTION;
             case 9:return IF;
             case 10:return INTEGER;
             case 11:return READLN;
             case 12:return REAL;
             case 13:return SORT;
             case 14:return STRING;
             case 15:return THEN;
             case 16:return TRUE;
             case 17:return VAR;
             case 18:return WHILE;
             case 19:return WRITE;

         }
     }
    }
    fseek(source,ftell(source)-1,SEEK_SET);
    return ID;
    }
}

///*****************************CONST STRINGS























if(c==EOF)
{   if(state!=0)
        return E_LEXICAL;
    else
        return SUCCESS;
}
if((isspace(c)==0)&&(state==0)&&(c!='}'))
{
  return E_LEXICAL;
}

    }

}


