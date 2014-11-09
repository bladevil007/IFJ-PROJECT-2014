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

int SyntacticAnalys ()
{

///alokacia pomocnej struktury lexikalnej analyzy

if(((LEX_STRUCTPTR =(LEX_STRUCT*)malloc(sizeof(LEX_STRUCT))) == NULL) ||
    (Init_str(LEX_STRUCTPTR)==E_INTERNAL))

return E_INTERNAL;




//test only,erase
int k=getnextToken(LEX_STRUCTPTR);

while((k!=SUCCESS) && (k!=E_LEXICAL)){
if(token==VAR)
token=declarelist();

if (token == BEGIN)
    token = prog();


k=getnextToken(LEX_STRUCTPTR);
}












free(LEX_STRUCTPTR);
return 0;
}


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


int prog()
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
        if(token==DOT)
            return token;
    }
}

int iffunction()
{
    int token;
    if((token = condition() == SUCCESS)
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

 int declarelist()
{

    int token=getnextToken(LEX_STRUCTPTR);
    printf("%i ide\n",token);
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


}*/
/*
int value()
{
 float value=LEX_STRUCTPTR->value



}

*/

