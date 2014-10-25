/* ****************************   ial.h  ************************************ */
/* Soubor:              ial.h - Hlavickovy soubor pro ial.c (Vestavene        */
/*						funkce a tabulka symbolu)							  */
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

#define FALSE 0
#define TRUE 1

int length(char *s);
char *copy(char *s, int i, int n);
int find(char *s, char *search);
char *sort(char *s);
