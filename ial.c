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

int length(char *s) {

	return strlen(s);
}

/* substring copy */
char *copy(char *s, unsigned i, unsigned n) { //funkce vraci ukazatel na podretezec, v pripade neuspechu volani malloc vraci NULL

    if (s != NULL) //pokud byl predan retezec
    {
		unsigned delka = strlen(s);
		char *r; //vysledny podretezec
		if (i >= 1 && i <= delka) //pokud je povoleny rozsah (nehleda se mimo retezec)
		{
			if ((r = malloc((n+1)*sizeof(char))) == NULL)
				return NULL;
			strncpy(r, s+i-1, n);

			if (n > 0)
				r[n]= '\0';
		}
		else //v tomto pripade se vraci prazdny retezec
		{
			if ((r = malloc(sizeof(char))) == NULL)
					return NULL;
			r[0] = '\0'; //v pripade, ze zacatek hledani retezce je az za retezcem, vraci se prazdny retezec
		}
		return r;
	}
	else //pokud predany retezec mel hodnotu NULL
	{
		return NULL;
	}

}

int *KMPFail (char *vzorek, int delka_vzorku) //funkce na vytvoreni vektoru FAIL ze vzorku - pouzivana u funkce FIND
{
	int j; //pomocne promenne
	int *fail; //vektor FAIL

	if((fail = malloc(sizeof(int)*delka_vzorku)) == NULL)
		return NULL;

	fail[0] = -1; //zacatek vektoru
	for (int i = 1; i < delka_vzorku; i++) //vytvareni vektoru
	{
		j = fail[i-1];
		while ((j > -1) && (vzorek[j] != vzorek[i-1]))
		{
			j = fail[j];
		}
		fail[i] = j+1;
	}
	return fail;
}


int matchKMP(char *text, char *vzorek, int delka_textu, int delka_vzorku) //funkce vraci pozici shody, v pripade chyby vraci -1, Funkce pouzita ve funkci find
{
	int *fail; //vektor fail
	int index_vzorku = 0;
	int index_textu = 0;

	if((fail = KMPFail(vzorek, delka_vzorku)) == NULL)
		return -1;

	while((index_vzorku < delka_vzorku) && (index_textu < delka_textu))
	{
		if((index_vzorku == -1) || (text[index_textu] == vzorek[index_vzorku]))
		{
			index_textu++;
			index_vzorku++;
		}
		else
		{
			index_vzorku = fail[index_vzorku];
		}
	}
	free(fail); //uvolneni vektoru fail
	if(index_vzorku >=delka_vzorku)
		return index_textu - delka_vzorku;
	else
		return index_textu;
}

int find (char *text, char *vzorek) //funkce pro vyhledani podretezce v retezci vraci -1 pri chybe
{
	int vysledek;
	if(text != NULL && vzorek != NULL)
	{
		if(strcmp(vzorek, "") != 0) //pokud hledany vzorek neni prazdny retezec
		{
			int delka_textu = strlen(text);
			int delka_vzorku = strlen(vzorek);
			if ((vysledek = matchKMP(text, vzorek, delka_textu, delka_vzorku)) == -1) //pokud doslo k chybe
			return -1;
			if (vysledek == delka_textu)
				return 0;
			else
				return vysledek +1;
		}
		else //pokud je hledany podretezec prazdny retezec - nachazi se na pozici 1
			return 1;
	}
	else
		return -1;
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

	if (s != NULL)
	{
		int sl = length(s); //zjisteni delky retezce
		char *result;

		result = copy(s, 1, sl); //zkopirovani retezce do nove promenne

		heapSort(result, sl); //serazeni

		return result;
	}
	else //pokud byl jako parametr predan NULL
	{
		return NULL;
	}
}
