/* ****************************   ial.c  ************************************ */
/* Soubor:              ial.c - Vestavene funkce a tabulka symbolu            */
/* Kodovani:            UTF-8                                                 */
/* Datum:               11.2014                                               */
/* Predmet:             Formalni jazyky a prekladace (IFJ)                    */
/* Projekt:             Implementace interpretu jazyka IFJ14                  */
/* Varianta zadani:     a/2/II                                                */
/* Titul,Autori, login:         Ing. Natalya Loginova   xlogin00              */
/*                              Jindrich Dudek          xdudek04              */
/*                              Norbert Durcansky       xdurca01              */
/*                              Jan Jusko               xjusko00              */
/*                              Jiri Dostal             xdosta40              */
/* ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* calculation of string length */
int length(char *s) {

	return strlen(s);
	
}

/* substring copy */
char *copy(char *s, int i, int n)
{
    
    char *r;
    r = (char) malloc((n+1)*sizeof(char));
    
    strncpy(r, s+i-1, n);
    
    if (n > 0)
    	r[n]= '\0';    
	
	return r;
}

/* substring search */
int find(char *s, char *search)
{

}

/* string sort */
char *sort(char *s)
{

}

