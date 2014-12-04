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
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "string.h"
#include "parser.h"
#include "scaner.h"
#include "err.h"

#define INIT_SIZE 20 //Velikost inicializovaneho pole, pri reallocu budeme pole zvetsovat o tuto hodnotu

//VSECHNY FUNKCE PRI NEUSPECHU VRACI HODNOTU 1, PRI USPECHU HODNOTU 0

Tpole_int *IntArrayInit() //inicializace pole
{
   Tpole_int *p;
   if ((p = malloc(sizeof(Tpole_int))) == NULL) //inicializace struktury
	  return NULL;
   if ((p->pole = malloc(sizeof(int)*INIT_SIZE)) == NULL) //inicializace pole
   {
      free(p);
      return NULL;
   }
   p->volny_index = 0;
   p->velikost_pole = INIT_SIZE;
   return p;
}

int IntArrayAddItem(Tpole_int *p, int i) //prida hodnotu do pole
{
   if (p->volny_index == p->velikost_pole) //pokud uz neni misto
   {
      if ((p->pole = realloc(p->pole, (p->velikost_pole + INIT_SIZE)*sizeof(int))) == NULL) //realokace, v pripade chyby se automaticky uvolni cela struktura i s polem
      {
         IntArrayStructFree(p);
         return 1;
      }
      p->velikost_pole = p->velikost_pole + INIT_SIZE;
   }
   p->pole[p->volny_index] = i; //vlozeni prvku
   p->volny_index++;
   return 0;
}

void IntArrayFree(Tpole_int *p) //uvolneni pole, POUZIVA SE POUZE POKUD CHCEME POLE ZRUSIT A ZNOVU INICIALIZOVAT, SAMOTNE UVOLNENI CELE STRUKTURY MA NA STAROST NALSEDUJICI FUNKCE
{
   free(p->pole);
}

void IntArrayStructFree(Tpole_int *p) //uvolneni pole a nasledne cele struktury
{
	IntArrayFree(p);
	free(p);
}


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
    if((a->str = (char*) malloc(100*sizeof(char))) == NULL)
        return -1;

    a->str[0] = '\0';
    a->length = 0;
    a->act_pos = 0;
    a->allocSize = 100;    ///opravit velkost lebo hadze chyby

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
int add_str_param(inf_array *a, char* strin)
{
    if((a->allocSize - a->length) <= (strlen(strin) + 1))       //ak je nedostatok volneho miesta v poli tak realokuj
    {
        if ((a->str = (char*) realloc(a->str, a->allocSize + ARR_INC + strlen(strin))) == NULL)
            return -1;

        a->allocSize = a->allocSize + ARR_INC + strlen(strin);                  //aktualizacia premennej
    }


    int i;
    int ret_val = a->act_pos;                                   //navratova hodnota

    for (i=0; i<strlen(strin); i++)                             //pridavanie retazca po znakoch
    {
        a->str[a->act_pos + i] = strin[i];
    }

    a->str[a->act_pos + strlen(strin)] = '$';                  //ukoncit nulovym znakom

    a->act_pos = a->act_pos + (strlen(strin) + 1);              //aktualizovat premenne v strkture
    a->length = a->length + strlen(strin);

    return ret_val;
}

/** \brief  Funkcia pridava string do nekonecneho pola
            Navratova hodnota je index v poli, na ktorom zacina pridany string
*/
int add_str(inf_array *a, char* strin)
{
    if((a->allocSize - a->length) <= (strlen(strin) + 1))       //ak je nedostatok volneho miesta v poli tak realokuj
    {
        if ((a->str = (char*) realloc(a->str, a->allocSize + ARR_INC + strlen(strin))) == NULL)
            exit(E_INTERNAL);

        a->allocSize = a->allocSize + ARR_INC + strlen(strin);                  //aktualizacia premennej
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

char *concatenate(const char *s1, const char *s2) //funkce na konkatenaci dvou retezcu, vraci NULL v pripade chyby
{
	char *concatenated; //ukazatel na konkatenovany retezec
	if (s1 != NULL && s2 != NULL) //pokud ani jeden z retezcu neni NULL
	{
		if (strcmp(s1, "") == 0) //pokud je s1 prazdny retezec vysledkem konkatenace je pouze retezec s2
		{
			if ((concatenated = malloc(sizeof(char)*(strlen(s2)+1))) == NULL) //alokace mista pro zkopirovani retezce s2
				return NULL;
			strcpy(concatenated, s2);
			return concatenated;
		}
		if (strcmp(s2, "") == 0) //pokud je s2 prazdny retezec, vysledkem konkatenace je pouze retezec s1
		{
			if ((concatenated = malloc(sizeof(char)*(strlen(s1)+1))) == NULL)
				return NULL;
			strcpy(concatenated, s1);
			return concatenated;
		}
		//pokud ani jeden z retezcu neni prazdny:
		int length_s1 = strlen(s1); //delka prvniho retezce
		int length = length_s1 + strlen(s2) + 1; //delka pro alokovani retezce (+1 kvuli nulovemu znaku)
		if ((concatenated = malloc(sizeof(char)*length)) == NULL)
			return NULL;
		strcpy(concatenated, s1);//zkopirovani prvni casti retezce
		strcpy(concatenated + length_s1, s2); //zkopirovani druhe casti retezce
		return concatenated;
	}
	else //pokud je jeden z ukazatelu NULL
	{
		return NULL;
	}
}

