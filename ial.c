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
char *copy(char *s, int i, int n) {
    
    char *r;
    r = (char) malloc((n+1)*sizeof(char));
    
    strncpy(r, s+i-1, n);
    
    if (n > 0)
    	r[n]= '\0';    
	
	return r;
	
}

/* FAIL vector for KMP algorithm */
void failKMP(char *p, int pl, int *fail) {

	int k, r;
	
	fail[0] = 0;
	
	for (k = 1; k < pl; k++) {
	
		r = fail[k-1];
		
		while (r > 0 && p[r] != p[k-1]) {
		
			r = fail[r];
		
		}
		
		fail[k] = r + 1;
	
	}

}

/* KMP algorithm */
int matchKMP(char *search, char *txt, int pl, int tl, int *fail) {

	int pi, ti;

	pi = 1;
	ti = 0;
	
	while (ti <= tl && pi <= pl) {
	
		if (pi = ??? || txt[ti] == search[pi]) {
		
			pi++;
			ti++;
		
		}
		
		else {
		
			pi = fail[pi];
		
		}
		
		if (pi > pl) {
		
			matchKMP = ti - pl;
		
		}
		
		else {
		
			matchKMP = ti;
		
		}
	
	}

}

/* substring search */
int find(char *s, char *search) {

	int pl, tl, result;
	
	pl = length(search);
	tl = length(s);
	
	int *fail;
    fail = (int) malloc(pl*sizeof(int));
	
	result = matchKMP(search, s, pl, tl, fail);
	
	if (result == tl + 1) {
	
		return 0;
	
	}
	
	else {
	
		return result;
	
	}

}

/* string sort */
char *sort(char *s) {

}

