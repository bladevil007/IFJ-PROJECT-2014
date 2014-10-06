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

int getnextToken ()    // parameter sa bude predavat ukazatel na pole znakov
{
    char c;   // premenna do ktorej si ukladame znak
    int state=0;   // stav v ktorom sme
    int ccount=0; //ulozime pocet nacitanych znakov

    while(1)
    {
        c=getc(source);
        ccount++;

// switch pre unarne operatory bez potreby pola pocet znakov max 1
        if(ccount==1)
        {
            switch(c)
            {
                case '+': return PLUS;
                case '-': return MINUS;
                case '*': return MULTIPLY;
                case '/': return DIVIDE;
                case ':': return APOSTROF;
                case ';': return BODKOCIARKA;
                case '=': return EQUAL;
                case '{': state=LEFT;
                    break; //oznacime si zaciatok komentu
                case '}': return E_SYNTAX;
                case '<': return LESS;
                case '>': return GREATER;
            }
        }

/*KOMENT*/
        if((c=='}')&&(state==LEFT))
            return KOMENT;













































        if ((c == EOF)&& (state == LEFT))
            return E_SYNTAX; //syntakticka chyba nedokonceny koment


        if ((c == EOF) && (state == 0))    /*na konci suboru vrati Token EOFILE*/
            return EOFILE;

    }



}



