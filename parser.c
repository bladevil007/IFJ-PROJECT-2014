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
#include "stack.h"
#include "precedent.h"

LEX_STRUCT *LEX_STRUCTPTR;
int IF_ENABLE=0;///na pouzitie else v progcondition

///funckia na overenie LEXIKALNA vs SYNTAKTICKA CHYBA
int ERRORRET(int value)
{
    return ((value==E_LEXICAL) ? E_LEXICAL : E_SYNTAX);
}


////<PROGRAM>  <FUNCTION> v <VAR> v <PROG>
int program(int token)
{
    ///program zacina var a pokracuje begin bez funkcii
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
    }else
    return ERRORRET(token);

}
///program obsahuje len telo hlavneho programu
else if(token==BEGIN)
    {
        token=prog();
        if(token==E_SYNTAX || token==E_LEXICAL)
            return ERRORRET(token);
    }
 ///program sa sklada z funkcii a programu
else if(token == FUNCTION)
    {
        token = funkcia();
        if(token == E_SYNTAX || token == E_LEXICAL)
            return ERRORRET(token);

    }
if(token==SUCCESS && (token=getnextToken(LEX_STRUCTPTR))==EOFILE)
    return SUCCESS;
else
    return ERRORRET(token);

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





///telo programu pre TELA cyklov  IF a While
int progcondition()
{
   int token;
    token = getnextToken(LEX_STRUCTPTR);
  ///vstavane funkcie copy,length,find,sort
    switch(token)
    {
        case ID:
        case WHILE:
        case IF:
        case  LENGTH:
        case  COPY:
        case  FIND:
        case  SORT:
        case  WRITE:
        case  READLN:
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
            return SUCCESS;
        }
        else if(token==BODKOCIARKA)
        return progcondition();
        else
            return ERRORRET(token);
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
        case END:
        break;
        case ID:
        case WHILE:
        case IF:
        case  LENGTH:
        case  COPY:
        case  FIND:
        case  SORT:
        case  WRITE:
        case  READLN:
        {
            token=command(token);
            if(token==E_SYNTAX || token==E_LEXICAL) return ERRORRET(token);
            break;
        }
        default:
            return ERRORRET(token);
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
        else if(token==BODKOCIARKA)
        return prog();
        ///if podmienka s else + vnorene IF
        else if(token==ELSE && IF_ENABLE==1)
        {
             IF_ENABLE=0;
             token=getnextToken(LEX_STRUCTPTR);
          if(token==BEGIN)
          {
            token=progcondition();
                if(token==SUCCESS)
                {
                token=getnextToken(LEX_STRUCTPTR);
                if(token==END)
                {
                token=getnextToken(LEX_STRUCTPTR);
                if(token==DOT)
                return SUCCESS;
                }
                else if(token==BODKOCIARKA)
                return prog();

                else
                    return ERRORRET(token);
                }else
                return ERRORRET(token);

          }else
            return ERRORRET(token);

        }
        else
            return ERRORRET(token);
}
else if(token==END)
{
token=getnextToken(LEX_STRUCTPTR);
if(token==DOT)
return SUCCESS;
else
    return ERRORRET(token);
}

}


/** <command>  |  <id>  := <hodnota> v <cykly> v <vstavanafunkcia> v <definovanefunkcie>
*/
int command(int value)
{
    int token;
    ///Vstavane funkcie
    if(value==COPY || value==FIND ||value==LENGTH || value==SORT)
        return Libraryfunction(value);

    else if(value==ID)
    {
        token=getnextToken(LEX_STRUCTPTR);
        if(token==DVOJBODKA)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==EQUAL)
            {
                /////// PRECEDENCNA TABULKA VYHODNOTI DANY ID
             return PrecedenceSA(LEX_STRUCTPTR,ID);

            }else return ERRORRET(token);

        }else return ERRORRET(token);
    }
    else if(value==READLN)

{
        token=getnextToken(LEX_STRUCTPTR);
        if (token==LEFT_ROUND)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==ID)
            {
                token=getnextToken(LEX_STRUCTPTR);
                if(token==RIGHT_ROUND)
                    return SUCCESS;
                else
                    return ERRORRET(token);
            }
            else
                return ERRORRET(token);
        }
        else
            return ERRORRET(token);

    }
    else if(value==IF)
    {

        ///dopisat
        IF_ENABLE=1;
        //return PrecedenceSA(LEX_STRUCTPTR);  VYHODNOTI SA PODMIENKA + NACITA SA THEN

        if((token=getnextToken(LEX_STRUCTPTR))==BEGIN)
      {
          return progcondition();
      }else ERRORRET(token);
    }
    else if(value==WHILE)
    {
        //return PrecedenceSA(LEX_STRUCTPTR);  VYHODNOTI SA PODMIENKA + NACITA SA do
        return progcondition();

    }
    ///write funkcia pre lubovolny pocet parametrov
    else if(value==WRITE)
    {
        token=getnextToken(LEX_STRUCTPTR);
        if(token==LEFT_ROUND)
        {
         return Libraryfunction(WRITE);

        }else ERRORRET(token);
    }

}




///Vstavane funkcie COPY...
int Libraryfunction(int value){
    int token;

if(value==WRITE)
{

    token=getnextToken(LEX_STRUCTPTR);
            if(token==ID || token==CONST_STRING)
            {
              token=getnextToken(LEX_STRUCTPTR);
              if(token==CIARKA)
              {
                  return Libraryfunction(value);

              }
              else if(token==RIGHT_ROUND)
                return SUCCESS;

              else ERRORRET(token);
            }else ERRORRET(token);

}
else if(value==LENGTH)
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
}



///VAR DEKLARACIA GLOBALNYCH PREMENNYCH  < VAR >
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
                    {
                        token=getnextToken(LEX_STRUCTPTR);
                        ///pridat do tabulky
                        {
                         if(token==BODKOCIARKA)
                         {
                         return declarelist();
                         }
                         else return ERRORRET(token);
                        }
                    }
                    else return ERRORRET(token);
           }
           else return ERRORRET(token);
    }
    else return token;
}




///DEFINOVANIE FUNKCIE
int funkcia()
{
    int token;
    if((token = getnextToken(LEX_STRUCTPTR)) == ID)
    {
        if ((token = getnextToken(LEX_STRUCTPTR)) == LEFT_ROUND)
        {
                token = fun_params();
                if(token==E_SYNTAX || token==E_LEXICAL)
                    return ERRORRET(token);

                if(token == RIGHT_ROUND)
                {
                    token = getnextToken(LEX_STRUCTPTR);

                }
        }
        else return E_SYNTAX; // za ID nenasleduje zatvorka
    }
    else return E_SYNTAX; //za function nenasleduje ID
}

int fun_params()
{
    int token;
    token = getnextToken(LEX_STRUCTPTR);
    if(token == RIGHT_ROUND)
        return token;
    else if(token == ID)
    {
        token = getnextToken(LEX_STRUCTPTR);
        if(token == DVOJBODKA)
        {
            token = getnextToken(LEX_STRUCTPTR);
            if (token == INTEGER || token  == BOOLEAN || token == STRING || token == REAL)
            {
                token = getnextToken(LEX_STRUCTPTR);
                if(token == BODKOCIARKA)
                {
                    return fun_params();
                }
                else if(token == RIGHT_ROUND)
                {
                    return token;
                }
                else return E_SYNTAX; // po DTYPE nenasleduje ani zatvorka ani bodkociarka
            }
            else return E_SYNTAX; //po dvojbodke nenasleduje DTYPE
        }
        else return E_SYNTAX; //po ID nenasleduje dvojbodka
    }

    else return E_SYNTAX;   // po zatvorke nenasleduje ID ani zatvorka
}


























