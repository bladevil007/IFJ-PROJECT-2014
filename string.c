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
#include <string.h>
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
   return strcmpi(LEX_STRUCTPTR->str,CONST_STR);
}





