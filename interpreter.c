#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "string.h"
#include "parser.h"
#include "scaner.h"
#include "err.h"
#include "interpreter.h"
#include "codegenerate.h"
#include "ial.h"
struct record *temp;
char *globalne_pole=0;





int foo(INSTape *INSTR)
{
    switch(INSTR->CODE)
    {
    case LENGTH:
        hodnota = length(INSTR->a);
        break;

    case MOV:
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        switch(temp->type)
        {
        case(INTEGER_hash):
            temp->value.i = (int)hodnota;
            break;
        case(REAL_hash):
            temp->value.d = hodnota;
            break;
        case(BOOLEAN_hash):
            temp->value.b = hodnota;
            break;
        case(STRING_hash):
           free(temp->value.str);
           temp->value.str = malloc(sizeof(char)*length(INSTR->a));
           if(temp->value.str == NULL)
              exit(E_INTERNAL);
          strcpy(temp->value.str,globalne_pole);
          free(globalne_pole);
          globalne_pole=NULL;
        }break;


    case WRITESTRING:
        printf("%s",INSTR->a);
        break;

    case WRITEBOOL:
        if((int)INSTR->b == 39)
            printf("TRUE");
        else if((int)INSTR->b == 38)
            printf("FALSE");
            break;

    case WRITEINT:
        printf("%i",(int)INSTR->b);
        break;

    case WRITEREAL:
        printf("%g",INSTR->b);
        break;

    case READLN:
    temp = hashtable_search(GlobalnaTAB,INSTR->a);
    switch(temp->type)
       {
        case(INTEGER_hash):
            if(scanf("%i",&(temp->value.i))==0)
                exit(E_STDIN);
                break;
        case(REAL_hash):
            if(scanf("%f",&(temp->value.d))==0)
            exit(E_STDIN);
            break;
        case(CHAR_hash):
            if(scanf("%c",&(temp->value.c))==0)
            exit(E_STDIN);
            break;
        case(STRING_hash):
            free(temp->value.str);
            temp->value.str=(char*)malloc(sizeof(char)*256);
            if(scanf("%s",(temp->value.str))==0)
            exit(E_STDIN);
            break;
        }
         break;
    case WRITEID:
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        switch(temp->type)
        {
        case(INTEGER_hash):
            printf("%i",(int)temp->value.i);
            break;
        case(REAL_hash):
            printf("%f",temp->value.d);
            break;
        case(STRING_hash):
            printf("%s",temp->value.str);
            break;
        case (BOOLEAN_hash):
            if((int)temp->value.b == 39)
                printf("TRUE");
            else if((int)temp->value.b == 38)
                printf("FALSE");
                break;
        }
        break;

        case COPYSTRING:
        globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
        globalne_pole = copy(INSTR->a, INSTR->b, INSTR->c);
        break;

        case COPYID:
        globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        globalne_pole = copy(temp->value.str, INSTR->b, INSTR->c);
        break;

      case SORTID:
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        if((globalne_pole = (malloc(length(temp->value.str) * sizeof(char) + 1))) == NULL)
            exit(E_INTERNAL);
        globalne_pole = sort(temp->value.str);
        break;

    case SORTSTRING:
        if((globalne_pole = malloc(length(INSTR->a) * sizeof(char) + 1)) == NULL)
            exit(E_INTERNAL);
        globalne_pole = sort(INSTR->a);
        break;

    case CONCATESTRING:
            if(globalne_pole==0){
            globalne_pole = (char*)malloc(sizeof(char) * (length(INSTR->a)));
            tempstr = concatenate("",INSTR->a);
            strcpy(globalne_pole, tempstr);
             }
           else
            {
                globalne_pole = (char*)realloc(globalne_pole, length(globalne_pole) + length(INSTR->a)+1);
                tempstr = concatenate(globalne_pole,INSTR->a);
                strcpy(globalne_pole, tempstr);
            }
        break;

    case CONCATEID:

        temp = hashtable_search(GlobalnaTAB,INSTR->a);

            if(globalne_pole==0){
            globalne_pole = (char*)malloc(sizeof(char) * (length(temp->value.str)));
            tempstr = concatenate("",temp->value.str);
            strcpy(globalne_pole, tempstr);
             }
           else
            {
                globalne_pole = (char*)realloc(globalne_pole, length(globalne_pole) + length(temp->value.str)+1);
                tempstr = concatenate(globalne_pole,temp->value.str);
                strcpy(globalne_pole, tempstr);
            }
        break;

        case LENGTHID:
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        hodnota = length(temp->value.str);
        break;

    }
return 0;
}
int searchrecord(inf_pointer_array* beh_programu)
{

    int i=0;


    while(beh_programu->pole[i]->CODE != BEGIN_MAIN)
    {
        i++;
    }

    i++;


    while(beh_programu->pole[i]->CODE != END_MAIN)
    {
        foo(beh_programu->pole[i]);
        i++;

    }

    return SUCCESS;
}




