/* **************************** string.c ************************************ */
/* Soubor:              string.c - podporna struktura pre lexikalny analyzator*/
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
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include "string.h"
#include "parser.h"
#include "scaner.h"
#include "err.h"


/** \brief Funkcia inicializuje pole
*/

int Init_str(LEX_STRUCT *LEX_STRUCTPTR)
{
   if ((LEX_STRUCTPTR->str = (char*) malloc(STRLEN)) == NULL)
      return E_INTERNAL;


  LEX_STRUCTPTR->str[0] = '\0';
  LEX_STRUCTPTR->length = 0;
  LEX_STRUCTPTR->mallocsize =STRLEN ;
   return SUCCESS;
}

/** \brief Funkcia uvolnuje pole
*/

void Free_str(LEX_STRUCT *LEX_STRUCTPTR)
{
   free(LEX_STRUCTPTR->str);
}


/** \brief Funkcia cisti pole
*/

void strClear(LEX_STRUCT *LEX_STRUCTPTR)
{
   LEX_STRUCTPTR->length = 0;
   LEX_STRUCTPTR->str[0]='\0';
}


/** \brief Funkcia prida znak do pola
*/

int AddChar_str(LEX_STRUCT *LEX_STRUCTPTR, char c)
{
   if (LEX_STRUCTPTR->length + 1 >= LEX_STRUCTPTR->mallocsize)
   {

      if ((LEX_STRUCTPTR->str = (char*) realloc(LEX_STRUCTPTR->str, LEX_STRUCTPTR->length + STRLEN )) == NULL)
         return E_INTERNAL;
      LEX_STRUCTPTR->mallocsize = LEX_STRUCTPTR->length + STRLEN;
   }
   LEX_STRUCTPTR->str[LEX_STRUCTPTR->length] = c;
   LEX_STRUCTPTR->length++;
   LEX_STRUCTPTR->str[LEX_STRUCTPTR->length] = '\0';
   return SUCCESS;
}


/** \brief Funkcia porovnava pole a const
*/
int CmpConst_str(LEX_STRUCT *LEX_STRUCTPTR, char* CONST_STR)
{
   return strcasecmp(LEX_STRUCTPTR->str,CONST_STR);
}


/** \brief Funkcia alokuje pole a inicializuje jeho strukturu
*/
int init_array(inf_array *a)
{
    if((a->str = (char*) malloc(ARR_INC)) == NULL)
        return -1;

    a->str[0] = '\0';
    a->length = 0;
    a->act_pos = 0;
    a->allocSize = ARR_INC;

    return 1;
}


/** \brief Funkcia uvolni alokovanu pamat
*/
void free_array(inf_array *a)
{
    free(a->str);
}


/** \brief  Funkcia pridava string do nekonecneho pola
            Navratova hodnota je index v poli, na ktorom zacina pridany string
*/
int add_str(inf_array *a, char* strin)
{
    if((a->allocSize - a->length) <= (strlen(strin) + 1))       //ak je nedostatok volneho miesta v poli tak realokuj
    {
        if ((a->str = (char*) realloc(a->str, a->allocSize + ARR_INC)) == NULL)
            return -1;

        a->allocSize = a->allocSize + ARR_INC;                  //aktualizacia premennej
    }


    int i;
    int ret_val = a->act_pos;                                   //navratova hodnota

    for (i=0; i<strlen(strin); i++)                             //pridavanie retazca po znakoch
    {
        a->str[a->act_pos + i] = strin[i];
    }

    a->str[a->act_pos + strlen(strin)] = '\0';                  //ukoncit nulovym znakom

    a->act_pos = a->act_pos + (strlen(strin) + 1);              //aktualizovat premenne v strkture
    a->length = a->length + strlen(strin);

    return ret_val;
}


