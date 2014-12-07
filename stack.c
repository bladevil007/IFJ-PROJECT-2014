/* **************************** stack.c ************************************ */
/* Soubor:              stack.c -  Struktury zasobnikov                        */
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
#include "stack.h"

//V pripade zmeny datoveho typu, ktery se bude ukladat na zasobnik je nutne zmenit: datovy typ ve strukture stackElement, datovy typ parametru ve funkcich stack_push a stack_top

TStack *stack_init () //funkce, ktera inicializuje zasobnik, navratova hodnota je ukazatel na strukturu zasobniku
{
	TStack *tmp = NULL;
	if ((tmp = malloc(sizeof(TStack))) == NULL) //alokace zasobniku
	{
		fprintf(stderr, "Allocation error.\n");
		return NULL;
	}
	tmp->top = NULL; //inicializace prazdneho zasobniku
	return tmp;
}

int stack_push (TStack *stack, int data) //funkce pro vlozeni dat na zásobník, NUTNE ZMENIT DATOVY TYP VKLADANYCH DAT, funkce vrací 0 v pripade uspesneho vlozeni na zasobnik
{
	stackElement *tmp = NULL;
	if ((tmp = malloc(sizeof(stackElement))) == NULL) //alokace prvku na zasobniku
	{
		fprintf(stderr,"Allocation Error.\n");
		return 1;
	}
	tmp->data = data; //prirazeni dat na nove vznikly prvek zasobniku
	tmp->next = stack->top; //novy prvek bude ukazovat na vrchol zasobniku
	stack->top = tmp; //zmeni se vrchol zasobniku na nove vytvoreny prvek
	return 0;
}

void stack_pop (TStack *stack) //funkce pro vyjmuti prvku z vrcholu zasobniku
{
	stackElement *tmp = NULL;
	if (stack->top != NULL) //pokud je co popovat
	{
		tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);
	}
}

int stack_top (TStack *stack, int *element) //prostrednictvim parametru element vrati hodnotu na vrcholu zasobniku, NUTNE ZMENIT DATOVY TYP VKLADANYCH DAT, funkce vraci 0 v pripade, ze zasobnik neni prazdny, 1 pokud je prazdny
{
	if (stack->top != NULL) //pokud zasobnik neni prazdny
	{
		*element = stack->top->data;
		return 0;
	}
	else
	{
		fprintf(stderr, "Stack is empty, can't read from top.\n");
		return 1;
	}
}

void stack_free(TStack *stack) //funkce ktera uvolni zasobnik
{
	while (stack->top != NULL) //pokud dojde k chybě při běhu programu, je nutné uvolnit data na zásobníku
	{
		stackElement *tmp;
		tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);
	}
	free (stack);
}
