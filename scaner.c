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

#include "scaner.h"
#include "parser.h"
#include "err.h"

FILE *source;
char STACKCH;


/** \brief funkcia inicializuje zdrojovy subor
 *
 * \param  File pointer
 *
 */

void SourceInitialize(FILE *f)
{

source=f;

}

/** \brief funkcia vrati dalsi token , volana syntaktickou analyzou
 * \param  char pointer
 * \return Token a klic v poli alebo  lexikalna chyba
 */

int getnextToken (TOKEN *TOKENptr)    // parameter sa bude predavat ukazatel na strukturu TOKEN  viz scaner.h + ukazatel na pole
{
  TOKENptr->druh=0;
  TOKENptr->value=0;
  TOKENptr->first_index=0;

char c;   // premenna do ktorej si ukladame znak
int state=0;   // stav v ktorom sme
int ccount=0; //ulozime pocet nacitanych znakov

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
    TOKENptr->druh=LESS;

    return SUCCESS;
case '>' :
    TOKENptr->druh=GREATER;
    return SUCCESS;
case '=' :
    TOKENptr->druh=EQUAL;
    return SUCCESS;
case '.' :
    TOKENptr->druh=DOT;
    return SUCCESS;
case ':' :
    TOKENptr->druh=DVOJBODKA;
     return SUCCESS;
case ';' :
    TOKENptr->druh=BODKOCIARKA;
     return SUCCESS;
case '+' :
    TOKENptr->druh=PLUS;
     return SUCCESS;
case '-' :
    TOKENptr->druh=MINUS;
     return SUCCESS;
case '/' :
    TOKENptr->druh=DIVIDE;
     return SUCCESS;
case '*' :
   TOKENptr->druh=MULTIPLY;
    return SUCCESS;
case '(' :
    TOKENptr->druh=LEFT_ROUND;
    return SUCCESS;
case ')' :
    TOKENptr->druh=RIGHT_ROUND;
    return SUCCESS;
case 39 :
    TOKENptr->druh=APOSTROF;
    return SUCCESS;
case '[' :
    TOKENptr->druh=LEFT_HRANATA;
    return SUCCESS;
case ']' :
    TOKENptr->druh=RIGHT_HRANATA;
    return SUCCESS;
/**case 'e' :
case 'E' :
   state=EXPONENT;*/
}
}




///CISLA A KONSTANTY
/*
if(state==EXPONENT)
{
 if((c >= '0') && (c <='9'))
 {
     TOKENptr->druh=EXPONENT;
    return SUCCESS;
    ///posunut ukazatel
 }

    ///uloz do pola >>>>><<<<<<<<
}
*/

if((c >= '0') && (c <='9')){
state=CONST;
TOKENptr->value=(TOKENptr->value*10)+(c-'0');
}

if((c < '0') || (c >'9')){
if(state==CONST){
 if(isspace(c)!=0)
{
    TOKENptr->druh=CONST;
    return SUCCESS;
 }

else if((c=='.') || (c==';') || ( c== 'e') || (c=='E') ||(c=='+') || (c=='-') || (c=='*') || (c=='/'))
 {
     fseek(source,ftell(source)-1,SEEK_SET);
     TOKENptr->druh=CONST;
     return SUCCESS;
 }
 else
    if(c==EOF)
 {
TOKENptr->druh=CONST;
return SUCCESS;
 }
 else
    return E_LEXICAL;

}
}
///***********************************************************






































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



