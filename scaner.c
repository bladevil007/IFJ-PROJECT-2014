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
/*                              Jiøí Dostál             xdosta40              */
/* ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "scaner.h"

FILE *source;


char keywords[]={ "begin","end"
};




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

int getnextToken ()    // parameter sa bude predavat ukazatel na pole znakov
{
    char c;   // premenna do ktorej si ukladame znak

    while(1)
    {

      c=getc(source);

 // switch pre unarne operatory bez potreby pola

      switch(c)
      {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MULTIPLY;
        case '/': return DIVIDE;
        case ':': return APOSTROF;
        case ';': return BODKOCIARKA;
        case '=': return EQUAL;
        case '{': return LEFT_VINCULUM;
        case '}': return RIGHT_VINCULUM;
        case '<': return LESS;
        case '>': return GREATER;
      }













        return FAIL;           // ked ziaden z uvedenych znakov vratime lexikalnu chybu
    }



}



