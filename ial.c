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
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ial.h"

///Jindrova Hash tabulka
void stringtolower (char *key) //funkce ktera prevede vsechny znaky v retezci na male znaky
{
	int i = 0;
	while (key[i] != '\0')
	{
		key[i] = tolower(key[i]);
		i++;
	}
}

THash_table *hashtable_init (unsigned size) //funkce na inicializaci tabulky
{
	THash_table *tmp = NULL; //docasna promenna, do ktere se ulozi ukazatel na tabulku

	if ((tmp = malloc(sizeof(THash_table) + (sizeof(struct record *)*size))) == NULL)  // alokace pameti pro polozku hashtable_size ve strukture + size*velikost ukazatele na zaznam slova
	{
		fprintf(stderr, "Allocation error.\n");
		return NULL;
	}
	tmp->hashtable_size = size; //ulozeni poctu polozek pole do struktury
	for(unsigned i = 0; i < tmp->hashtable_size; i++) //jednotlive polozky pole se vyplni NULL pointerama
		tmp->ptr[i] = NULL;
	return tmp;
}

unsigned int hash_function (const char *str, unsigned hashtable_size)  //hashovaci funkce
{
	unsigned int h=0;
	const unsigned char *p;

	for(p=(const unsigned char*)str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h % hashtable_size;
}

void hashtable_clear (THash_table *table) //funkce, ktera uvolni vsechny zaznamy v tabulce
{
	for (unsigned i = 0; i < (table->hashtable_size); i++) //prochazeni vsech radku v tabulce
	{
		while (table->ptr[i] != NULL) //postupne uvolnovani linearniho seznamu od prvniho zaznamu
		{
			struct record *tmp;
			tmp = table->ptr[i];
			table->ptr[i] = tmp->next;
			if (tmp->params != NULL) //pokud se jedna o funkci a retezec params neni prazdny
			{
				free(tmp->params);
			}
			if (tmp->id == VARIABLE_hash && tmp->type == STRING_hash && tmp->defined == true_hash) //pokud je zaznam promenna typu string a ma prirazenou hodnotu je treba ji uvolnit
			{
				free(tmp->value.str);
			}
			free(tmp);
		}
	}
	return;
}

void hashtable_free(THash_table *table) //funkce na uvolneni cele tabulky
{
	hashtable_clear(table);
	free(table);
	return;
}

struct record *hashtable_search(THash_table *table, char *key) //funkce na vyhledani zaznamu
{
	unsigned int index = 0; //promenna, do ktere se ulozi index z hashovaci funkce
	stringtolower(key); //prevedeni retezce na mala pismena
	index = hash_function(key, table->hashtable_size); //zjisteni indexu v tabulce pro ulozeni zaznamu
	for (struct record *tmp = table->ptr[index]; tmp != NULL; tmp = tmp->next) //projizdeni jednotlivych zaznamu linearniho seznamu
	{
		if (strcmp(tmp->key, key) == 0) //porovna se hledane slovo se slovem ve strukture
		{
			return tmp;
		}
	}
	//pokud se program dostane do tohoto mista, znamena to, ze nebyla nalezena shoda se slovem:
	return NULL;
}

struct record *hashtable_add(THash_table *table, int id, char *key, int type, char *params) //funkce pro pridani zaznamu do tabulky
{
	unsigned int index = 0; //promenna, do ktere se ulozi index z hashovaci funkce
	stringtolower(key); //prevedeni retezce na mala pismena
	index = hash_function(key, table->hashtable_size); //zjisteni indexu v tabulce pro ulozeni zaznamu

	struct record *tmp; //pomocna promenna
	if ((tmp = malloc(sizeof(struct record))) == NULL) //pokud nelze alokovat pamet
	{
		fprintf(stderr, "Allocation error.\n");
		return NULL;
	}
	//pripojeni noveho zaznamu do seznamu
	tmp->next = table->ptr[index];
	table->ptr[index] = tmp;
	tmp->id = id; //pridani informaci do zaznamu
	tmp->key = key;
	tmp->type = type;
	tmp->params = params;
	tmp->defined = false_hash;
	return tmp;
}

///Nataly funkcie

/* calculation of string length */
int length(char *s) {

	return strlen(s);
}

/* substring copy */
/*char *copy(char *s, int i, int n) {

    char *r;
    r = (char) malloc((n+1)*sizeof(char));

    strncpy(r, s+i-1, n);

    if (n > 0)
    	r[n]= '\0';

	return r;
}

/* FAIL vector for KMP algorithm */
/*void failKMP(char *p, int pl, int *fail) {

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
/*int matchKMP(char *search, char *txt, int pl, int tl, int *fail) {

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
}*/

/* substring search */
/*int find(char *s, char *search) {

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
*/
/* sift down procedure for heap sort algorithm */
/*void siftDown(char *s, int left, int right) {

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
/*void heapSort(char *s, int n) {

	int left, right;
	char tmp;

	/* heapify */
	/*for (left = (n - 2)/2; left >= 0; left--) {

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
/*char *sort(char *s) {

	int sl = length(s);
	char *result;

    result = copy(s, 1, sl);

	heapSort(result, sl);

	return result;
}
*/
