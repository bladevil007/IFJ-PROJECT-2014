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
int state=0;   // stav v ktorom sme

    while(1)
    {

c=getc(source);
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
/*
if((c >= '0') && (c <='9')){
state=CONST;
LEX_STRUCTPTR->value=(LEX_STRUCTPTR->value*10)+(c-'0');
}

if((c < '0') || (c >'9')){
if(state==CONST){
 if(isspace(c)!=0)
{
    return CONST;
 }
if(c=='-')
state=REALM;

if(c=='.')
state=DOT;


if(c=='+')
state=REAL;

if((c=='e') || (c=='E'))
state=REALo;

else if (c==';')|| (c=='*') || (c=='/'))
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








///*********************************************************** identifikatory a klucove slova





































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


