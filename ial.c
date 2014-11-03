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
#include <strings.h>

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

	fail[0] = -1;

	for (k = 1; k < pl; k++) {
		r = fail[k-1];

		while (r > -1 && p[r+1] != p[k])
			r = fail[r];

		fail[k] = r+1;
	}
}

/* KMP algorithm */
int matchKMP(char *search, char *txt, int pl, int tl, int *fail) {

	int pi, ti;

	pi = 0;
	ti = 0;

	while (ti < tl && pi < pl) {

		if (pi == -1 || txt[ti] == search[pi]) {
			pi++;
			ti++;
		}
		else
			pi = fail[pi];

		if (pi >= pl)
			matchKMP = ti - pl + 1;
		else
			matchKMP = ti;
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

	free (fail);

	if (result == tl)
		return 0;

	else
		return result;
}

/* sift down procedure for heap sort algorithm */
void siftDown(char *s, int left, int right) {

	int i, j;
	char tmp;

	i = left;
	j = 2*i + 1;

	while (j < right) {

		if ((j + 1 < right) && (s[j] < s[j+1])) {
			j = j + 1;
		}

		if (s[i] < s[j]) {

			tmp = s[i];
			s[i] = s[j];
			s[j] = tmp;

			i = j;
			j = 2*i + 1;
		}

		else
			return;
	}
}

/* HeapSort algorithm */
void heapSort(char *s, int n) {

	int left, right;
	char tmp;

	/* heapify */
	for (left = (n - 2)/2; left >= 0; left--) {

		siftDown(s, left, n);
	}

	for (right = n - 1; right > 0; right--) {

		tmp = s[0];
		s[0] = s[right];
		s[right] = tmp;

		siftDown(s, 0, right);
	}
}

/* string sort */
char *sort(char *s) {

	int sl = length(s);
	char *result;

    result = copy(s, 1, sl);

	heapSort(result, sl);

	return result;
}

