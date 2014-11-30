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
struct record *temp2;
struct record *temp3;
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
            printf("%g",temp->value.d);
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


        case ADD:
            if(INSTR->specialcode==0)
            if(hodnota==0)
            hodnota=INSTR->b+INSTR->c;
            else
            hodnota2=INSTR->b+INSTR->c;
            else if(INSTR->specialcode==1)
            {
                hodnota=hodnota+INSTR->b;
            }
            else if(INSTR->specialcode==3)
            {
                hodnota=hodnota+INSTR->b;
            }
            else if(INSTR->specialcode==4)
            {
              hodnota=hodnota+hodnota2;
            }
            break;

        case MULTIPLY:
            if(INSTR->specialcode==0)
            {if(hodnota==0)
            {hodnota=INSTR->b*INSTR->c;
             hodnota3=1;
            }
            else
            {
            hodnota2=INSTR->b*INSTR->c;
            hodnota3=2;
            }}
            else if(INSTR->specialcode==1)
            {
                if(hodnota3==1 || hodnota3==0)
                hodnota=hodnota*INSTR->b;
                else
                hodnota2=hodnota2*INSTR->b;
            }
            break;


        case MINUS:
            if(INSTR->specialcode==0)
            {if(hodnota==0)
            hodnota=INSTR->b-INSTR->c;
            else
            hodnota2=INSTR->b-INSTR->c;}
            else if(INSTR->specialcode==1)
            {
                hodnota=hodnota-INSTR->b;
            }
            else if(INSTR->specialcode==3)
            {
                hodnota=INSTR->b-hodnota;
            }
            else if(INSTR->specialcode==4)
            {
              hodnota=hodnota-hodnota2;
            }
          break;

       case DIVIDE:

             if(INSTR->specialcode==0)
             {
            if(hodnota==0)
            {hodnota=INSTR->b/INSTR->c;
             if(INSTR->c==0)
                exit(E_DIVISION_BY_ZERO);
             hodnota3=1;
            }
            else
            {
            hodnota2=INSTR->b/INSTR->c;
             if(INSTR->c==0)
                exit(E_DIVISION_BY_ZERO);
            hodnota3=2;
            }}
            else if(INSTR->specialcode==1)
            {
                if(hodnota3==1 || hodnota3==0)
                hodnota=hodnota/INSTR->b;
                else
                hodnota2=hodnota2/INSTR->b;
                if(INSTR->b==0)
                exit(E_DIVISION_BY_ZERO);
            }
            break;

       case EQUAL:
            hodnota=0;
            hodnota2=0;
            break;




            ///retazce




        case COPYSTRINGID_:         //copy('ahoj', i, 52);
            globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            globalne_pole = copy(INSTR->a, temp->value.i, INSTR->c);
            break;

        case COPYSTRING_ID:         //copy('ahoj', 52, i);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            globalne_pole = malloc(temp->value.i * sizeof(char) + 1);
            globalne_pole = copy(INSTR->a, INSTR->b, temp->value.i);
            break;

        case COPYSTRINGIDID:         //copy('ahoj', 52, i);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
            globalne_pole = malloc(temp2->value.i * sizeof(char) + 1);
            globalne_pole = copy(INSTR->a, temp->value.i, temp2->value.i);
            break;

        case COPYIDID_:             //copy(id,id,52);
            globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
            globalne_pole = copy(temp->value.str, temp2->value.i, INSTR->c);
            break;

        case COPYID_ID:             //copy(id,52,id);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
            globalne_pole = malloc(temp2->value.i * sizeof(char) + 1);
            globalne_pole = copy(temp->value.str, INSTR->b, temp2->value.i);
            break;

        case COPYIDIDID:             //copy(id,id,id);
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
            temp3 = hashtable_search(GlobalnaTAB,INSTR->a3);
            globalne_pole = malloc(temp3->value.i * sizeof(char) + 1);
            globalne_pole = copy(temp->value.str, temp2->value.i, temp3->value.i);
            break;

        case FINDSTRSTR:
            hodnota = find(INSTR->a,INSTR->a2);
            break;

        case FINDIDSTR:
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            hodnota = find(temp->value.str, INSTR->a2);
            break;

        case FINDSTRID:
            temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            hodnota = find(INSTR->a,temp->value.str);
            break;

        case FINDIDID:
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
            temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
            hodnota = find(temp->value.str,temp2->value.str);
            break;




















///*-+/ hotovo bez premennych


    }
return 0;
}
int searchrecord(inf_pointer_array* beh_programu)
{
hodnota2=0;
hodnota=0;
hodnota3=0;

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




