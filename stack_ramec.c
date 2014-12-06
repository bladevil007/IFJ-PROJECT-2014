#include <stdio.h>
#include <stdlib.h>
#include "ial.h"

//V pripade zmeny datoveho typu, ktery se bude ukladat na zasobnik je nutne zmenit: datovy typ ve strukture stackElement, datovy typ parametru ve funkcich stack_push a stack_top

TStack_ramec *stack_init_ramec () //funkce, ktera inicializuje zasobnik, navratova hodnota je ukazatel na strukturu zasobniku
{
	TStack_ramec *tmp = NULL;
	if ((tmp = malloc(sizeof(TStack_ramec))) == NULL) //alokace zasobniku
	{
		fprintf(stderr, "Allocation error.\n");
		return NULL;
	}
	tmp->top = NULL; //inicializace prazdneho zasobniku
	return tmp;
}

int stack_push_ramec (TStack_ramec *stack, THash_table *data) //funkce pro vlozeni dat na zásobník, NUTNE ZMENIT DATOVY TYP VKLADANYCH DAT, funkce vrací 0 v pripade uspesneho vlozeni na zasobnik
{
	stackElement_ramec *tmp = NULL;
	if ((tmp = malloc(sizeof(stackElement_ramec))) == NULL) //alokace prvku na zasobniku
	{
		fprintf(stderr,"Allocation Error.\n");
		return 1;
	}
	tmp->data = data; //prirazeni dat na nove vznikly prvek zasobniku
	tmp->next = stack->top; //novy prvek bude ukazovat na vrchol zasobniku
	stack->top = tmp; //zmeni se vrchol zasobniku na nove vytvoreny prvek
	return 0;
}

void stack_pop_ramec (TStack_ramec *stack) //funkce pro vyjmuti prvku z vrcholu zasobniku
{
	stackElement_ramec *tmp = NULL;
	if (stack->top != NULL) //pokud je co popovat
	{
		tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);
	}
}

int stack_top_ramec (TStack_ramec *stack, THash_table **element) //prostrednictvim parametru element vrati hodnotu na vrcholu zasobniku, NUTNE ZMENIT DATOVY TYP VKLADANYCH DAT, funkce vraci 0 v pripade, ze zasobnik neni prazdny, 1 pokud je prazdny
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

void stack_free_ramec(TStack_ramec *stack) //funkce ktera uvolni zasobnik
{
	while (stack->top != NULL) //pokud dojde k chybě při běhu programu, je nutné uvolnit data na zásobníku
	{
		stackElement_ramec *tmp;
		tmp = stack->top;
		stack->top = tmp->next;
		free(tmp);
	}
	free (stack);
}

