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


/* convertion of escape sequence to integer */
int escSekv(char *s, int *index) {
	int res = 0;
	int len = 0;
	int pow = 0;
	int i;

	while(s[*index] == '0')	{
		(*index)++;
	}
	
	while(s[*index] >= '0' && s[*index] <= '9')	{
		len++;
		(*index)++;
	}
	
	if(len > 3)	{
		fprintf(stderr, "Error: too long escape sequence\n");
		return 0;
	}
	
	for(i = 0; i < len; i++) {
	
		switch (i) {
			case 0: pow = 1; break;
			case 1: pow = 10; break;
			case 2: pow = 100; break;
		}
		
		res = res + (s[index - 1 - i] - '0')*pow;
	}
	return res;
}


/* calculation of string length */
int length(char *s) {

	int i = 0;
	int esc = 0;
	int len = 0;

	while(i < strlen(s)) {
		if(s[i] == '#')
		{
			i++;
			esc = escSekv(s, &i);
			
			if(esc >= 1 && esc <= 255) {
				len++;
				esc = 0;
			}
			
			else {
				fprintf(stderr, "Error: wrong escape sequence\n");
				return 0;
			}
		}
		
		if(s[i] == '#')	{
			continue;
		}
		
		if(s[i] >= ' ' && s[i] != '\'')	{
			len++;	
		}
		
		if(s[i] == s[i - 1] && s[i] == '\'')
		{
			len++;
			i++;
		}
		i++;
	}
	return len;
}

/* substring copy */
char copy(char *s, int i, int n)
{
	//char r[]; 
	//int j=0;
	//
	//for (j=0; j<n; j++)
	//	r[j]=s[j+i-1];
	//for ( ; j<n; j++)
    //    dest[i] = '\0';
	
	return r;
}

/* substring search */
int find(char *s, char *search)
{

}

/* string sort */
char sort(char *s)
{

}

