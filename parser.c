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
#include "err.h">

int SyntacticAnalys ()
{
TOKEN *TOKENptr;
if(((TOKENptr=(TOKEN*)malloc(sizeof(TOKEN )))==NULL) ||(((TOKENptr->druh=malloc(sizeof(int)))==NULL)) ||
  ((TOKENptr->value=malloc(sizeof(int)))==NULL) ||((TOKENptr->first_index=malloc(sizeof(int)))==NULL))
        return E_INTERNAL;




/* test only
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));
printf("%i  %i TOKEN \n",TOKENptr->druh,getnextToken(TOKENptr));

*/


free(TOKENptr); //TEMPORARY
























return 0;
}
