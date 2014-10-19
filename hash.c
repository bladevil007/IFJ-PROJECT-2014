#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

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
			if (tmp->id == VARIABLE && tmp->type == STRING && tmp->defined == true) //pokud je zaznam promenna typu string a ma prirazenou hodnotu je treba ji uvolnit
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
	//nejdrive se zjisti, zda se v tabulce nenachazi stejny identifikator:
	unsigned int index = 0; //promenna, do ktere se ulozi index z hashovaci funkce

	index = hash_function(key, table->hashtable_size); //zjisteni indexu v tabulce pro ulozeni zaznamu
	for (struct record *tmp = table->ptr[index]; tmp != NULL; tmp = tmp->next) //projizdeni jednotlivych zaznamu linearniho seznamu
	{ 
		if (strcmp(tmp->key, key) == 0) //porovna se hledane slovo se slovem ve strukture
		{ 
			fprintf(stderr, "ERROR: Identifier is already in symbol table.\n");
			return NULL; //pokud uz se stejny identifikator vyskytl v tabulce, vraci NULL -> chyba   
		}   
	}
	//pokud se program dostane do tohoto mista, znamena to, identifikator zatim v tabulce neexistuje:
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
	tmp->defined = false;
	return tmp;
}
