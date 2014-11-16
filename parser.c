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

///funckia na overenie LEXIKALNA vs SYNTAKTICKA CHYBA
int ERRORRET(int value)
{
    return ((value==E_LEXICAL) ? E_LEXICAL : E_SYNTAX);
}


////<PROGRAM>  <FUNCTION> v <VAR> v <PROG>
int program(int token)
{
    if(token==VAR)
    {
        token=declarelist();
    if(token==E_SYNTAX || token==E_LEXICAL)
        return ERRORRET(token);
    if(token==BEGIN)
    {
        token=prog();
        if(token==E_SYNTAX || token==E_LEXICAL)
            return ERRORRET(token);
    }
}

if(token==SUCCESS && (token=getnextToken(LEX_STRUCTPTR))==EOFILE)
return SUCCESS;
else return ERRORRET(token);

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
if(token==EOFILE)return SUCCESS;
int ok=program(token);
if(ok==SUCCESS)
    return SUCCESS;
else
    return ERRORRET(ok);
strClear(LEX_STRUCTPTR);
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

///<prog>  BEGIN <PRIKAZ> V <CYKLY> V <DEFINOVANA FUNKCIA> V <VSTAVANAFUNKCIA> END.
///nonterminal prog definuje oddelenie hlavneho programu begin ----> end.
int prog()
{
    int token;
    token = getnextToken(LEX_STRUCTPTR);
  ///vstavane funkcie copy,length,find,sort
    switch(token)
    {
        case  LENGTH:
        case  COPY:
        case  FIND:
        case  SORT:
        {
            token=command(token);
            if(token==E_SYNTAX || token==E_LEXICAL) return ERRORRET(token);
            break;
        }
        default: return ERRORRET(token);
    }

    if(token==SUCCESS)
{
        token=getnextToken(LEX_STRUCTPTR);
        if(token==END)
        {
        token=getnextToken(LEX_STRUCTPTR);
        if(token==DOT)
            return SUCCESS;
        }
        else return prog();
}
}




/** <command>  |  <id>  := <hodnota> v <cykly> v <vstavanafunkcia> v <definovanefunkcie>
*/
int command(int value)
{
    int token;
    ///Vstavane funkcie
    if(value==LENGTH)
    {
     token=getnextToken(LEX_STRUCTPTR);
     if(token==LEFT_ROUND)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==CONST_STRING || token==ID)
            {
            token=getnextToken(LEX_STRUCTPTR);
                if(token==RIGHT_ROUND)
                    return SUCCESS;
                else return ERRORRET(token);

            }else return ERRORRET(token);

        }else return ERRORRET(token);
    }

    else if(value==COPY)
    {
       token=getnextToken(LEX_STRUCTPTR);
       if(token==LEFT_ROUND)
       {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==CONST_STRING || token==ID)
            {
                token=getnextToken(LEX_STRUCTPTR);
                if(token==CIARKA)
                {
                  token=getnextToken(LEX_STRUCTPTR);
                  if(token==ID || token== CONST)
                  {
                    token=getnextToken(LEX_STRUCTPTR);
                        if(token==CIARKA)
                        {
                         token=getnextToken(LEX_STRUCTPTR);
                                if(token==ID || token==CONST)
                                {
                                   token=getnextToken(LEX_STRUCTPTR);
                                   if(token==RIGHT_ROUND)
                                        return SUCCESS;
                                     else return ERRORRET(token);
                                }else return ERRORRET(token);
                        }else return ERRORRET(token);
                  }else return ERRORRET(token);

                }else return ERRORRET(token);
            }else return ERRORRET(token);
       }else return ERRORRET(token);

    }
    else if(value==FIND)
    {
        token=getnextToken(LEX_STRUCTPTR);
        if(token==LEFT_ROUND)
        {
           token=getnextToken(LEX_STRUCTPTR);
                if(token==CONST_STRING || token==ID)
                {
                    token=getnextToken(LEX_STRUCTPTR);
                    if(token==CIARKA)
                    {
                         token=getnextToken(LEX_STRUCTPTR);
                         if(token==CONST_STRING || token==ID)
                         {
                            token=getnextToken(LEX_STRUCTPTR);
                            if(token==RIGHT_ROUND)
                                return SUCCESS;
                            else return ERRORRET(token);
                         }else return ERRORRET(token);
                    }else return ERRORRET(token);
                }else return ERRORRET(token);
        }else return ERRORRET(token);
    }
    else if(value==SORT)
    {
      token=getnextToken(LEX_STRUCTPTR);
      if(token==LEFT_ROUND)
      {
          token=getnextToken(LEX_STRUCTPTR);
          if(token==CONST_STRING || token==ID)
          {
              token=getnextToken(LEX_STRUCTPTR);
              if(token==RIGHT_ROUND)
                return SUCCESS;
                else return ERRORRET(token);
          }else return ERRORRET(token);
      }else return ERRORRET(token);
    }
	//15.11.2014 READLN, WRITE
	else if(value==READLN)
	
}

































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
                         else return E_SYNTAX;
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


