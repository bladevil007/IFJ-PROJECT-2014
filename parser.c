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
    if(value==E_LEXICAL)
        exit(E_LEXICAL);
    else
        exit(E_SYNTAX);
}

///<PROGRAM>  <FUNCTION> v <VAR> v <PROG>
/**
*Koren derivacneho stromu, neterminal,
 plati pre tokeny < var > or <begin> or  <function>
 alebo ich kombinacia pricom <function> musi zacinat ako prve
**/
int program(int token)
{
    ///program zacina var a pokracuje begin bez funkcii
    if(token==VAR)
    {
        token=declarelist();

    if(token==BEGIN)
    {
        token=prog();

    }else
    return ERRORRET(token);

}
///program obsahuje len telo hlavneho programu
else if(token==BEGIN)
    {
        token=prog();
    }

 ///program sa sklada z funkcii a programu
else if(token == FUNCTION)
    {
        funkcia();  // pri chybe program skonci sam
        token=getnextToken(LEX_STRUCTPTR);
        if(token==FORWARD)    /// len hlavicka funkcie ziadne telo za nou nenasleduje
        {
            token=getnextToken(LEX_STRUCTPTR);
                if(token==BODKOCIARKA)
                {
                    token=getnextToken(LEX_STRUCTPTR);
                    return program(token);
                }
                else
                    return ERRORRET(token);

        }
        else if(token==BEGIN || token==VAR)
        {
            if(token==VAR)
            {
            declarelist();
            }
            progfunction();
            token=getnextToken(LEX_STRUCTPTR);
            if(token==E_LEXICAL)exit(E_LEXICAL);
            ///bud ide dalsia funkcia alebo ide uz telo programu
            return program(token);
        }
        else
            return ERRORRET(token);
    }

if(token==SUCCESS && (token=getnextToken(LEX_STRUCTPTR))==EOFILE) /// za end. nesmie nasledovat nic ine
    return SUCCESS;
else
    return ERRORRET(token);

}
/***
Hlavna funkcia syntaktickej analyzi
Nepatri medzi neterminaly ale spusta dolezite akcie
**/
int SyntacticAnalys ()
{
ID_ENABLE = 0;
///alokacia pomocnej struktury lexikalnej analyzy
if(((LEX_STRUCTPTR =(LEX_STRUCT*)malloc(sizeof(LEX_STRUCT))) == NULL) ||
    (Init_str(LEX_STRUCTPTR)==E_INTERNAL))
exit(E_INTERNAL);

int token=getnextToken(LEX_STRUCTPTR); ///nacitame prvy token
if(token==E_LEXICAL)exit(E_LEXICAL);///pri chybe v lexikalnej analyze skonci
if(token==EOFILE) exit(E_SYNTAX);
int ok=program(token);
if(ok==SUCCESS)
    return SUCCESS;
strClear(LEX_STRUCTPTR);
free(LEX_STRUCTPTR);
return 0;
}/////////////////////////////////////////////********/*/*/*/


/***
///telo programu pre funkciu zacina begin a konci end;
Tento neterminal sa vola len pri prijati terminalu/tokenu <function>
**/
int progfunction()
{
   int token;
    token = getnextToken(LEX_STRUCTPTR);
  ///vstavane funkcie copy,length,find,sort + ostatne mozne prikazy
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
               if(token==BODKOCIARKA)
                    return SUCCESS;
               else return ERRORRET(token);
        }
        else if(token==BODKOCIARKA)
        return progfunction();
        else
            return ERRORRET(token);
}else if(token==SUCCESS && ID_ENABLE == 1)
{
	ID_ENABLE = 0;
	return progfunction();
}else if(token==SUCCESS && ID_ENABLE == 2)
{
	ID_ENABLE = 0;
	token=getnextToken(LEX_STRUCTPTR);
    if(token==BODKOCIARKA)
		return SUCCESS;
	else return ERRORRET(token);
}
return ERRORRET(token);
}
/***
///telo programu pre TELA cyklov  IF a While, su to neterminaly volane neterminalom/funckiou
"command".Tento neterminal sa vola len pri prijati terminalu/tokenu <if> alebo <while>
pretoze su ukoncene "end" bez bodky ci strednika
**/
int progcondition()
{
   int token;
    token = getnextToken(LEX_STRUCTPTR);
  ///vstavane funkcie copy,length,find,sort + ostatne mozne prikazy
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
            break;
        }
        default: return ERRORRET(token);
    }


    if(token==SUCCESS && ID_ENABLE == 0)
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
}else if(token==SUCCESS && ID_ENABLE == 1)
{
	ID_ENABLE = 0;
	return progcondition();
}else if(token==SUCCESS && ID_ENABLE == 2)
{
	ID_ENABLE = 0;
	return SUCCESS;
}
return ERRORRET(token);
}

/***
///<prog>  BEGIN <PRIKAZ> V <CYKLY> V <DEFINOVANA FUNKCIA> V <VSTAVANAFUNKCIA> END.
Hlavne telo MAIN  program oddeleny Begin a end. pricom obsahuje prikazy.
**/
int prog()
{
    int token;
    token = getnextToken(LEX_STRUCTPTR);
  ///vstavane funkcie copy,length,find,sort+dalsie
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
            break;
        }
        default:
            return ERRORRET(token);
    }
    if(token==SUCCESS && ID_ENABLE == 0)
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
}else if (token == SUCCESS && ID_ENABLE == 1)
{
	ID_ENABLE = 0;
	return prog();
}else if (token == SUCCESS && ID_ENABLE == 2)
{
	ID_ENABLE = 0;
	token=getnextToken(LEX_STRUCTPTR);
                if(token==DOT)
                return SUCCESS;
}
return ERRORRET(token);
}


/** <command>  |  <id>  := <hodnota> v <cykly> v <vstavanafunkcia> v <definovanefunkcie>
Neterminal/funckia Command , ktora je volana z neterminalov progcondition/prog/progfunction
Pri chybe sa hned vola exit(cislo chyby)
*/


int command(int value)
{
    int token;
    ///Vstavane funkcie
    if(value==COPY || value==FIND ||value==LENGTH || value==SORT)
        return Libraryfunction(value);

    else if(value==ID)  /// riesit zvlast lebo konci  strednikem
    {
        token=getnextToken(LEX_STRUCTPTR);
        if(token==DVOJBODKA)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==EQUAL)
            {

				PrecedenceSA(LEX_STRUCTPTR,ID);
				return SUCCESS;
            }else return ERRORRET(token);

        }else return ERRORRET(token);
    }

    ///Prikaz readln
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
    ///IF podmienka sa vyhodnocuje zvlast v precendencnej analyze
    else if(value==IF)
    {

        ///dopisat
       IF_ENABLE=1;
       PrecedenceSA(LEX_STRUCTPTR,IF);///  VYHODNOTI SA PODMIENKA + NACITA SA THEN

        if((token=getnextToken(LEX_STRUCTPTR))==BEGIN)
      {
          return progcondition();
      }else ERRORRET(token);
    }
    ///WHILE CYKLUS + VNORENE WHILE CYKLY
    else if(value==WHILE)
    {
        PrecedenceSA(LEX_STRUCTPTR,WHILE);  //VYHODNOTI SA PODMIENKA + NACITA SA do

            if((token=getnextToken(LEX_STRUCTPTR))==BEGIN)
      {
          return progcondition();
      }else ERRORRET(token);

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

return ERRORRET(token);
}



/**
///Vstavane funkcie COPY...
Neterminal Libraryfunction je volana z neterminalu command kontroluje syntax vstavanych funckii
*/
int Libraryfunction(int value){
    int token;


if(value == FUNCTION)
{
	token=getnextToken(LEX_STRUCTPTR);
            if(token==ID || token==CONST_STRING || token == TRUE || token == CONST || token==FALSE || token==REALo)
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

else if(value==WRITE)
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
                                if(token==ID || token==CONST )
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
return ERRORRET(token);
}



///VAR DEKLARACIA GLOBALNYCH PREMENNYCH  < VAR >
/**
Neterminal na kontrolu syntaxe deklaracie premennych
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
/**
Neterminal volany z neterminalu "program" ktory kontroluje syntax funckie
*/
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
                    if(token==DVOJBODKA)
                    {
                     token = getnextToken(LEX_STRUCTPTR);
                        if (token == INTEGER || token  == BOOLEAN || token == STRING || token == REAL)
                        {
                         token = getnextToken(LEX_STRUCTPTR);
                            if(token==BODKOCIARKA)
                            {
                                return SUCCESS;               ///hlavicka je OK
                            }else return ERRORRET(token);

                        }else return ERRORRET(token);
                    }else return ERRORRET(token);


                }else return ERRORRET(token);
        }
        else return ERRORRET(token); // za ID nenasleduje zatvorka
    }
    else return ERRORRET(token); //za function nenasleduje ID
}

/**
Neterminal ktory je volany neterminalom "funkcia()"
Tato funkcia kontroluje sytax parametrov vo vnutri zatvorky
*/
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
                else return ERRORRET(token); // po DTYPE nenasleduje ani zatvorka ani bodkociarka
            }
            else return ERRORRET(token); //po dvojbodke nenasleduje DTYPE
        }
        else return ERRORRET(token); //po ID nenasleduje dvojbodka
    }

    else return ERRORRET(token);   // po zatvorke nenasleduje ID ani zatvorka
}


























