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

int SyntacticAnalys ()
{
LEX_STRUCT *LEX_STRUCTPTR;
///alokacia pomocnej struktury lexikalnej analyzy

if(((LEX_STRUCTPTR =(LEX_STRUCT*)malloc(sizeof(LEX_STRUCT))) == NULL) ||
    (Init_str(LEX_STRUCTPTR)==E_INTERNAL))
return E_INTERNAL;




//test only,erase 
int k=getnextToken(LEX_STRUCTPTR);

while((k!=SUCCESS) && (k!=E_LEXICAL)){
printf("%iX",k);
printf("%.2fX",LEX_STRUCTPTR->value);
printf("%s\n",LEX_STRUCTPTR->str);
k=getnextToken(LEX_STRUCTPTR);
}
























free(LEX_STRUCTPTR);
return 0;
}
