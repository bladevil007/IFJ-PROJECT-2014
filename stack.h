#ifndef STACK
#define STACK

typedef struct stackElement //element ulozeny na zasobniku
{
	int data; //zde se ZMENI datovy typ podle toho, co bude obsahem zasobniku
	struct stackElement *next;	
} stackElement;


typedef struct  //struktura pro zasobnik
{
	stackElement *top;	
} TStack; 

//prototypy funkci:
TStack *stack_init ();
int stack_push (TStack *stack, int data);
void stack_pop (TStack *stack);
int stack_top (TStack *stack, int *element);
void stack_free(TStack *stack);

#endif
