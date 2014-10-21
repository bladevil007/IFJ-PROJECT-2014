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
///ODSTRANENIE KOMENTOV
if(c=='{' )
    state=KOMENT;
if ((state==KOMENT) && (c=='}'))
    state=0;

///STAVY TVORENE JEDNOU LEXEMOU
if(state==0){
switch(c) {
case '<' :
    return LESS;
case '>' :
       return GREATER;
case '=' :
    return EQUAL;
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

return ID;
}






























if(c==EOF)
{   if(state!=0)
        return E_LEXICAL;
    else
        return SUCCESS;
}
/// DOPLNIT LEXEMY
///

    }

}


