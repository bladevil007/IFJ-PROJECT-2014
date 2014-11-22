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
#include "ial.h"
#include "stack.h"
#include "precedent.h"

LEX_STRUCT *LEX_STRUCTPTR;

int IF_ENABLE=0;                ///na pouzitie else v progcondition
inf_array *POLE_ID_GLOBAL;   ///nekonecne pole ID pre globalnu
inf_array *POLE_ID_LOCAL;   ///nekonecne pole ID pre lokalnu

inf_array *SUPPORT_POLE;  /// pomocna
THash_table *GlobalnaTAB; ///globalna hashovacia tabulka
THash_table *LokalnaTAB; ///lokalna hashovacia tabulka

int POLE_ID_INDEX=0;  ///Index Dalsieho zaciatku ID
struct record *ELEMENT;   ///zaznam pre hashovaciu funkciu

int IN_FUNCTION=0;    ///nachadzame sa vo funckii budeme naplnat Lokalnu tab symbolov


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
        hashtable_clear(LokalnaTAB);                                 ///pokazde vymaze tabulku symbolov pre kazdu funkciu
        funkcia();                                                  /// pri chybe program skonci sam
        token=getnextToken(LEX_STRUCTPTR);
        if(token==FORWARD)                                          /// len hlavicka funkcie ziadne telo za nou nenasleduje
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
            IN_FUNCTION=1;
            declarelist();

            }
            progfunction();
            token=getnextToken(LEX_STRUCTPTR);
            if(token==E_LEXICAL)exit(E_LEXICAL);
            ///bud ide dalsia funkcia alebo ide uz telo programu
            IN_FUNCTION=0;
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

if(((POLE_ID_GLOBAL=(inf_array*)malloc(sizeof(inf_array))) == NULL) ||
    (init_array(POLE_ID_GLOBAL)==-1))                                        ///alokujeme pole ID glob
exit(E_INTERNAL);

if(((POLE_ID_LOCAL=(inf_array*)malloc(sizeof(inf_array))) == NULL) ||
    (init_array(POLE_ID_LOCAL)==-1))                                        ///alokujeme pole ID loc
exit(E_INTERNAL);

if(((SUPPORT_POLE=(inf_array*)malloc(sizeof(inf_array))) == NULL) ||
    (init_array(SUPPORT_POLE)==-1))                                        ///alokujeme pole ID
exit(E_INTERNAL);

if(((GlobalnaTAB=(THash_table*)malloc(sizeof(THash_table))) == NULL) ||
    ((GlobalnaTAB=hashtable_init(100))==0))                                        ///alokujeme hashovaciu tabulku
exit(E_INTERNAL);


if(((LokalnaTAB=(THash_table*)malloc(sizeof(THash_table))) == NULL) ||
    ((LokalnaTAB=hashtable_init(100))==0))                                        ///alokujeme hashovaciu tabulku
exit(E_INTERNAL);




if((ELEMENT=(struct record*)malloc(sizeof(struct record))) == NULL)          ///alokujeme hashovaciu tabulku
exit(E_INTERNAL);


int token=getnextToken(LEX_STRUCTPTR); ///nacitame prvy token
if(token==E_LEXICAL)exit(E_LEXICAL);///pri chybe v lexikalnej analyze skonci
if(token==EOFILE) exit(E_SYNTAX);
int ok=program(token);
if(ok==SUCCESS)
    return SUCCESS;
strClear(LEX_STRUCTPTR);
free_array(POLE_ID_GLOBAL);
free_array(POLE_ID_LOCAL);
hashtable_free(GlobalnaTAB);
hashtable_free(LokalnaTAB);
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

    else if(value==ID)
    {
            if(IN_FUNCTION==0)///Kontrola ci je definovana
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));

                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);

        token=getnextToken(LEX_STRUCTPTR);
        if(token==DVOJBODKA)
        {
            token=getnextToken(LEX_STRUCTPTR);
            if(token==EQUAL)
            {
				PrecedenceSA(LEX_STRUCTPTR,ID,GlobalnaTAB,LokalnaTAB);
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
       PrecedenceSA(LEX_STRUCTPTR,IF,GlobalnaTAB,LokalnaTAB);///  VYHODNOTI SA PODMIENKA + NACITA SA THEN



        if((token=getnextToken(LEX_STRUCTPTR))==BEGIN)
      {
          return progcondition();
      }else ERRORRET(token);
    }
    ///WHILE CYKLUS + VNORENE WHILE CYKLY
    else if(value==WHILE)
    {
        PrecedenceSA(LEX_STRUCTPTR,WHILE,GlobalnaTAB,LokalnaTAB);  //VYHODNOTI SA PODMIENKA + NACITA SA do

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
                if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }
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
                  if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }

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
                  if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }

                token=getnextToken(LEX_STRUCTPTR);
                if(token==CIARKA)
                {
                  token=getnextToken(LEX_STRUCTPTR);
                  if(token==ID || token== CONST)
                  {
                        if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su INT
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=INTEGER_hash)
                        exit(E_SEMANTIC_TYPE);
                }


                    token=getnextToken(LEX_STRUCTPTR);
                        if(token==CIARKA)
                        {
                         token=getnextToken(LEX_STRUCTPTR);
                                if(token==ID || token==CONST )
                                {
                                            if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su INT
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=INTEGER_hash)
                        exit(E_SEMANTIC_TYPE);
                }
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
                       if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }

                    token=getnextToken(LEX_STRUCTPTR);
                    if(token==CIARKA)
                    {
                         token=getnextToken(LEX_STRUCTPTR);
                         if(token==CONST_STRING || token==ID)
                         {
                                if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }

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
                if(token==ID)
                {
                    ///Kontrola ze vsetky vstupne hodnoty su String
                    if(IN_FUNCTION==0)
                    ELEMENT=(hashtable_search(GlobalnaTAB,LEX_STRUCTPTR->str));
                    else
                    ELEMENT=(hashtable_search(LokalnaTAB,LEX_STRUCTPTR->str));
                                                                                            ///Vrati na ukazatel na prvek v hash table
                        if(ELEMENT==0)
                        exit(E_SEMANTIC_UNDEF);
                        else if(ELEMENT->type!=STRING_hash)
                        exit(E_SEMANTIC_TYPE);
                }
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
         ///VKLADANIE DO TABULIEK SYMBOLOV
    if(IN_FUNCTION==0)
    {

       POLE_ID_INDEX=add_str(POLE_ID_GLOBAL,LEX_STRUCTPTR->str);                    ///ulozime ID do pola ID a ulozime si nove posunutie pre dalsi ID
                                                                                                    ///Zistime ci uz nemame taku polozku
        ELEMENT=((hashtable_search(GlobalnaTAB,POLE_ID_GLOBAL->str+POLE_ID_INDEX)));                    ///Vrati na ukazatel na prvek v hash table
         if(ELEMENT!=0)
            exit(E_SEMANTIC_UNDEF);                                                                     ///dva krat definovany ten isty nazov

    }else
    {
        POLE_ID_INDEX=add_str(POLE_ID_LOCAL,LEX_STRUCTPTR->str);                    ///ulozime ID do pola ID a ulozime si nove posunutie pre dalsi ID
                                                                                                    ///Zistime ci uz nemame taku polozku
        ELEMENT=((hashtable_search(LokalnaTAB,POLE_ID_LOCAL->str+POLE_ID_INDEX)));                    ///Vrati na ukazatel na prvek v hash table
         if(ELEMENT!=0)
            exit(E_SEMANTIC_UNDEF);                                                                     ///dva krat definovany ten isty nazov
    }
        if((token=getnextToken(LEX_STRUCTPTR))== DVOJBODKA)
           {
                        token=getnextToken(LEX_STRUCTPTR);
                    if(token==REAL || token==INTEGER || token==STRING || token == BOOLEAN)
                    {
                        if(IN_FUNCTION==0)
                        hashtable_add(GlobalnaTAB,VARIABLE_hash,POLE_ID_GLOBAL->str+POLE_ID_INDEX,decodederSEM(token),NULL);  ///pridame ID do tabulky symbolov GLOB
                         else
                        hashtable_add(LokalnaTAB,VARIABLE_hash,POLE_ID_LOCAL->str+POLE_ID_INDEX,decodederSEM(token),NULL);  ///pridame ID do tabulky symbolov    LOC

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


///Funckia dekoduje nase tokeny na hodnoty pre hashovaciu funkciu
int decodederSEM(int token)
{
    switch(token)
    {
    case INTEGER :
        return INTEGER_hash;
    case BOOLEAN:
        return BOOLEAN_hash;
    case STRING:
        return STRING_hash;
    case REAL:
        return REAL_hash;
    case TRUE:
        return true_hash;
    case FALSE:
        return false_hash;
    }
}






















