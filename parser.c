/* **************************** parser.c ************************************ */
/* Soubor:              parser.c - Syntakticka a Semanticka analyza           */
/* Kodovani:            UTF-8                                                 */
/* Datum:               11.2014                                               */
/* Predmet:             Formalni jazyky a prekladace (IFJ)                    */
/* Projekt:             Implementace interpretu jazyka IFJ14                  */
/* Varianta zadani:     a/2/II                                                */
/* Titul,Autori, login:         Ing. Natalya Loginova   xlogin00              */
/*                              Jindrich Dudek          xdudek04              */
/*                              Norbert Durcansky       xdurca01              */
/*                              Jan Jusko               xjusko00              */
/*                              Jiøí Dostál             xdosta40              */
/* ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"
#include "parser.h"
#include "string.h"
#include "err.h"

LEX_STRUCT *LEX_STRUCTPTR;


////<PROGRAM>  <FUNCTION> v <VAR> v <PROG>
int program(token)
{

   while(1){
if(token==VAR)
{
token=declarelist();
if(token==E_LEXICAL)return E_LEXICAL;
if(token==E_SYNTAX)return E_SYNTAX;

if(token==BEGIN)
{
 token=prog();
 printf("token %i",token);
if(token==E_LEXICAL)return E_LEXICAL;
if(token==E_SYNTAX)return E_SYNTAX;
}
}


if(token==SUCCESS)
    return SUCCESS;

}
}

///Hlavna funkcia syntaktickej analyzi
int SyntacticAnalys ()
{
///alokacia pomocnej struktury lexikalnej analyzy
if(((LEX_STRUCTPTR =(LEX_STRUCT*)malloc(sizeof(LEX_STRUCT))) == NULL) ||
    (Init_str(LEX_STRUCTPTR)==E_INTERNAL))
return E_INTERNAL;

int token=getnextToken(LEX_STRUCTPTR); ///nacitame prvy token
if(token==E_LEXICAL)return E_LEXICAL;///pri chybe v lexikalnej analyze skonci
if(token==SUCCESS)return SUCCESS;
int ok=program(token);
if(ok==SUCCESS)
    return SUCCESS;
if(ok==E_SYNTAX)
    return E_SYNTAX;
if(ok==E_LEXICAL)
    return E_LEXICAL;

free(LEX_STRUCTPTR);
return 0;
}/////////////////////////////////////////////********/*/*/*/


int progcondition()
{
    int token;
    token = getnextToken(LEX_STRUCTPTR);






    if (token == SUCCESS)
        return E_SYNTAX;

    if (token == E_LEXICAL)
        return E_LEXICAL;

    if (token == END)
    {
        token = getnextToken(LEX_STRUCTPTR);
        if(token==BODKOCIARKA)
            return token;
    }
}

///<prog>  BEGIN <PRIKAZ> V <CYKLY> END.
int prog()
{
    int token;
    token = getnextToken(LEX_STRUCTPTR);
//////////////////////////////////////////////////////////
if(token==LENGTH)  ///vstavana funkcia;
{
token=command(token);
if(token==E_LEXICAL)return E_LEXICAL;
if(token==E_SYNTAX)return E_SYNTAX;
}
else
    return E_SYNTAX;
/////////////////////////////////////////////////////////

    if (token == E_SYNTAX)
        return E_SYNTAX;

    if (token == E_LEXICAL)
        return E_LEXICAL;

    if(token==SUCCESS)
{
        token=getnextToken(LEX_STRUCTPTR);
        if(token==END)
            token=getnextToken(LEX_STRUCTPTR);
        if(token==DOT)
            return SUCCESS;
        else return prog();
}
}



int command(value)
{
    if(value==LENGTH)
    {
     int token=getnextToken(LEX_STRUCTPTR);
     if(token==LEFT_ROUND)
        token=getnextToken(LEX_STRUCTPTR);
        if(token==CONST_STRING || token==ID)
        token=getnextToken(LEX_STRUCTPTR);
            if(token==RIGHT_ROUND);
            return SUCCESS;
    }
}
/*****************************************































/*

int iffunction()
{
    int token;
    if((token = condition()) == SUCCESS)
    {
        progcondition();
        token=getnextToken(LEX_STRUCTPTR);
        if(token == ELSE)
            progcondition();
    }
    else
    {
        return E_SYNTAX;
    }
}
*/


 int declarelist()
{

    int token=getnextToken(LEX_STRUCTPTR);
    printf("%iIDE\n",token);
    if(token==ID)
    {
        ///ULOZIT NAZOV DO TABULKY
        if((token=getnextToken(LEX_STRUCTPTR))== DVOJBODKA)
           {
                        token=getnextToken(LEX_STRUCTPTR);
                    if(token==REAL || token==INTEGER || token==STRING || token == BOOLEAN)
                        token=getnextToken(LEX_STRUCTPTR);
                        ///pridat do tabulky

                        {
                         if(token==BODKOCIARKA)
                         {


                         return declarelist();
                         }
                         else E_SYNTAX;
                        }

           }
           else return E_SYNTAX;
    }
    else return token;
}


/*
int condition()
{
  int token=getnextToken(LEX_STRUCTPTR);
        if(token==LEFT_ROUND)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==ID)
            {

            }
            else
                if(token=CONST)
                    value();
        }


}
*/


