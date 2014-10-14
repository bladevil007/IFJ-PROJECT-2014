/* **************************** scaner.c ************************************ */
/* Soubor:              scaner.c - Lexikalna analyza                          */
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
#include "scaner.h"
#include "parser.h"
#include "err.h"

FILE *source;


/** \brief funkcia inicializuje zdrojovy subor
 *
 * \param  File pointer
 *
 */

void SourceInitialize(FILE *f)
{

source=f;

}

/** \brief funkcia vrati dalsi token , volana syntaktickou analyzou
 * \param  char pointer
 * \return Token a klic v poli alebo  lexikalna chyba
 */

int getnextToken ()    // parameter sa bude predavat ukazatel na strukturu TOKEN  viz scaner.h
{

char c;   // premenna do ktorej si ukladame znak
int state=0;   // stav v ktorom sme
int ccount=0; //ulozime pocet nacitanych znakov

    while(1)
    {







/// DOPLNIT LEXEMY
///
return 0;
    }

}



