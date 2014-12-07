/* **************************** interpreter.c ************************************ */
/* Soubor:             interpreter.c - Generovanie kodu a jeho vykonanie      */
/* Kodovani:            UTF-8                                                 */
/* Datum:               11.2014                                               */
/* Predmet:             Formalni jazyky a prekladace (IFJ)                    */
/* Projekt:             Implementace interpretu jazyka IFJ14                  */
/* Varianta zadani:     a/2/II                                                */
/* Titul,Autori, login:         Ing. Natalya Loginova   xlogin00              */
/*                              Jindrich Dudek          xdudek04              */
/*                              Norbert Durcansky       xdurca01              */
/*                              Jan Jusko               xjusko00              */
/*                              Jiøí Dostál             xdosta40              */
/* ****************************************************************************/
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
#include "stack.h"
int INFUN=0;                /// premenna ktora urcuje ci sa nachadzame vo funkcii alebo mimo nej
TStack *stackADRESS;        /// zasobik adries navesti
TStack *stackPC;            /// zasobnik adries pri viacerych volaniach funkcii
TStack *StackBreak;         ///  breakpoint pre while cyklus, kam sa vratit aku cast programu prevadzat
TStack_ramec *stackramec;    /// zasobnik ramcov, ukazatelov na tabulku symbolov
int LOOPER (inf_pointer_array* beh_programu,int BREAKPOINT,int i);     ///Hlavicka funkcie pre while cykly
int fun(inf_pointer_array* beh_programu,int j);  ///hlavicka funkcie pre prevadzanie tela funkcie
int TOP; ///Vrchol zasobnik StackAdress
struct record *temp;   /// pomocna premenna pre interpret
struct record *temp2;  /// pomocna premenna pre interpret
struct record *temp3;  /// pomocna premenna pre interpret
int RES;               /// pomocna premenna pre interpret
char *globalne_pole=NULL;  /// pomocna premenna pre interpret
char *globalne_pole1=NULL;  /// pomocna premenna pre interpret
int LABEL; /// pomocna premenna pre interpret



/** \brief Funkcia je pomocnou funkciou interpretu, kde sa prevadzaju vsetky instrukcie
 *
 * \param  ukazatel na Instrukiu
 * \return 0 alebo specialcode ktory riadi dalsie operacie
 *
 */
int foo(INSTape *INSTR)
{
THash_table *RAMEC; /// pomocna premenna pre interpret
int g1; /// pomocna premenna pre interpret
int g2; /// pomocna premenna pre interpret

    switch(INSTR->CODE)                   ///Podla Code vieme o aku instrukciu sa jedna
    {
    case LENGTH:                           ///instrukcia length vrati dlzku retazca /implementova v ial.c
        hodnota = length(INSTR->a);
        break;

    case MOV:                              /// instrukcia MOV sluzi na vkladanie hodnot o premennych
        if(INFUN==0)                       ///podla tohto flagu vieme kde sa nachadzame a kde mame hladat ID
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
        stack_top_ramec(stackramec,&RAMEC);                  ///nachadzame sa vo funkcii tak hladame v najvyssom Ramci na zasobniku
        temp=hashtable_search(RAMEC,INSTR->a);
        if(temp==NULL)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        switch(temp->type)                    ///podla typu  vieme kam danu hodnotu vlozit
        {

        case(INTEGER_hash):                    ///int
            temp->value.i = (int)hodnota;
            break;

        case(REAL_hash):                     ///float
            temp->value.d = hodnota;
            break;

        case(BOOLEAN_hash):                    ///boolean
            temp->value.b =(int)hodnota;
            temp->value.i=(int)hodnota-37;
            break;

        case(STRING_hash):                        ///string
           if(temp->value.str!=NULL)
                free(temp->value.str);
           temp->value.str = malloc(sizeof(char)*length(INSTR->a));
           if(temp->value.str == NULL)
           {
               free_sources();
               exit(E_INTERNAL);
           }
           strcpy(temp->value.str,globalne_pole);
           free(globalne_pole);
           globalne_pole=NULL;
           break;
        }
        break;

    case WRITESTRING:                         ///Instrukcia zapise string na stdout
        printf("%s",INSTR->a);
        break;

    case WRITEBOOL:                             ///Instrukcia zapise bool na stdout
        if((int)INSTR->b == 39)
            printf("TRUE");
        else if((int)INSTR->b == 38)                  ///hodnoty su dane podla hodnot tokenu zo scaner.c
            printf("FALSE");
        break;

    case WRITEINT:                               ///Instrukcia zapise Integer na stdout
        printf("%i",(int)INSTR->b);
        break;

    case WRITEREAL:
        printf("%g",INSTR->b);
        break;

    case READLN:                                ///Instrukcia nacitava hodnotu z stdin
        if(INFUN==0)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            stack_top_ramec(stackramec,&RAMEC);
            temp=hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        switch(temp->type)
        {
        case(INTEGER_hash):
            if(scanf("%i",&(temp->value.i))==0)      ///pri nespravnej hodnote program skonci
            {
                free_sources();
                exit(E_STDIN);
            }
            break;

        case(REAL_hash):
            if(scanf("%f",&(temp->value.d))==0)
            {
                free_sources();
                exit(E_STDIN);
            }
            break;

        case(BOOLEAN_hash):
            if(scanf("%i",&(temp->value.b))==0)
            {
                free_sources();
                exit(E_STDIN);
            }
            temp->value.b=temp->value.b+1;               ///true je 1 false je 0
            break;
        case(STRING_hash):
            if(temp->value.str!=NULL)                          ///vymazeme pole ak tam nieco je
                free(temp->value.str);
            temp->value.str=(char*)malloc(sizeof(char)*256);     ///size of one line
            if((scanf("%s",(temp->value.str)))==0)
            {
                free_sources();
                exit(E_STDIN);
            }
            break;
        }
        break;

    case WRITEID:                                   ///Instruckia zapise na stdout hodnotu ID
        if(INFUN!=1)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            THash_table *RAMEC;
            stack_top_ramec(stackramec,&RAMEC);
            temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
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
            if((int)temp->value.b == 2)
                printf("TRUE");
            else if((int)temp->value.b == 1)
                printf("FALSE");
            break;
        }
        break;

    case COPYSTRING:                                          ///Instrukcia kopiruje string a vklada ho do pomocnej premennej
        globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
        globalne_pole = copy(INSTR->a, INSTR->b, INSTR->c);
    break;

    case COPYID:                                                ///Instrukcia kopiruje string za pouzitia premennej
        globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
        if(INFUN!=1)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            THash_table *RAMEC;
            stack_top_ramec(stackramec,&RAMEC);
            temp = hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        globalne_pole = copy(temp->value.str, INSTR->b, INSTR->c);
        break;

    case SORTID:                                                        ///Instrukcia na prevedenie funckie sort
        if(INFUN!=1)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            THash_table *RAMEC;
            stack_top_ramec(stackramec,&RAMEC);
            temp = hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        if((globalne_pole = (malloc(length(temp->value.str) * sizeof(char) + 1))) == NULL)
        {
            free_sources();
            exit(E_INTERNAL);
        }
        globalne_pole = sort(temp->value.str);
        break;

    case SORTSTRING:
        if((globalne_pole = malloc(length(INSTR->a) * sizeof(char) + 1)) == NULL)
        {
            free_sources();
            exit(E_INTERNAL);
        }
        globalne_pole = sort(INSTR->a);
        break;

    case CONCATESTRING:                      ///konkatenacia stringov
            if(globalne_pole==0)
            {
                globalne_pole = (char*)malloc(sizeof(char) * (length(INSTR->a)));
                tempstr = concatenate("",INSTR->a);
                strcpy(globalne_pole, tempstr);
             }
           else
            {
                tempstr = concatenate(globalne_pole,INSTR->a);
                globalne_pole = (char*)realloc(globalne_pole, length(globalne_pole) + length(INSTR->a)+1);
                strcpy(globalne_pole,tempstr);
            }
        break;

    case CONCATEID:
        if(INFUN!=1)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            THash_table *RAMEC;
            stack_top_ramec(stackramec,&RAMEC);
            temp = hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        if(globalne_pole==0)
        {
            globalne_pole = (char*)malloc(sizeof(char) * (length(temp->value.str))+1);
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

    case LENGTHID:                                          ///Dlzka retazca z identifikatoru
        if(INFUN!=1)
            temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
            THash_table *RAMEC;
            stack_top_ramec(stackramec,&RAMEC);
            temp = hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
        hodnota = length(temp->value.str);
        break;

    case ADD:                                               ///Instruckia ADD sluzi na scitanie
        if(INSTR->a!=NULL)                                ///Instruckia prima 2x float a 2x pole. Ak pole neni prazdne tak ho berie ako identifikator a pouzije jeho hodnotu
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->b =temp->value.i;
            break;

            case REAL_hash:
            INSTR->b =temp->value.d;
            break;

            case BOOLEAN_hash:
            INSTR->b =temp->value.i;
            break;
            }
        }
        if(INSTR->a2!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a2);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->c =temp->value.i;
            break;

            case REAL_hash:
            INSTR->c =temp->value.d;
            break;

            case BOOLEAN_hash:
            INSTR->c =temp->value.i;
            break;
            }
        }
            if(INSTR->specialcode==0)                       ///Scitame  dve cisla , pomocne vysledky su nulove
            {
                if(hodnota==0)
                    hodnota=INSTR->b+INSTR->c;
                else
                    hodnota2=INSTR->b+INSTR->c;
            }
            else if(INSTR->specialcode==1)                  /// pripocitavame  cislo uz k vypocitanej hodnote
            {
                hodnota=hodnota+INSTR->b;
            }
            else if(INSTR->specialcode==3)
            {
                hodnota=hodnota+INSTR->b;
            }
            else if(INSTR->specialcode==4)                     ///robime sum dvoch uz vypocitanych hodnot
            {
              hodnota=hodnota+hodnota2;
            }

            break;

    case MULTIPLY:                                                      ///Instrukcia robi nasobenie s 2x float, 2xID
        if(INSTR->a!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a);
            }
            switch(temp->type)
            {
                case INTEGER_hash:
                INSTR->b =temp->value.i;
                break;

                case REAL_hash:
                INSTR->b =temp->value.d;
                break;
            }
        }
        if(INSTR->a2!=NULL)
        {
             if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a2);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->c =temp->value.i;
            break;

            case REAL_hash:
            INSTR->c =temp->value.d;
            break;
            }
        }
            if(INSTR->specialcode==0)
            {
                if(hodnota==0)
                {
                hodnota=INSTR->b*INSTR->c;
                hodnota3=1;
                }
                else
                {
                hodnota2=INSTR->b*INSTR->c;
                hodnota3=2;
                }
            }
            else if(INSTR->specialcode==1)
            {
                if(hodnota3==1 || hodnota3==0)
                    hodnota=hodnota*INSTR->b;
                else
                    hodnota2=hodnota2*INSTR->b;
            }
            break;


    case MINUS:                                   ///Instrukcia robi Minus s 2x float a 2x ID
        if(INSTR->a!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->b =temp->value.i;
            break;

            case REAL_hash:
            INSTR->b =temp->value.d;
            break;

            case BOOLEAN_hash:
            INSTR->b =temp->value.i;
            break;
            }
        }

        if(INSTR->a2!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a2);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->c =temp->value.i;
            break;

            case REAL_hash:
            INSTR->c =temp->value.d;
            break;

            }
        }
            if(INSTR->specialcode==0)
            {
                if(hodnota==0)
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

   case DIVIDE:                       ///Instrukcia robi DIV s 2xfloat a 2xID
        if(INSTR->a!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a);
            if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->b =temp->value.i;
            break;

            case REAL_hash:
            INSTR->b =temp->value.d;
            break;

            }
        }
        if(INSTR->a2!=NULL)
        {
            if(INFUN==0)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            else
            {
                stack_top_ramec(stackramec,&RAMEC);
                temp=hashtable_search(RAMEC,INSTR->a2);
                if(temp==NULL)
                    temp = hashtable_search(GlobalnaTAB,INSTR->a2);
            }
            switch(temp->type)
            {
            case INTEGER_hash:
            INSTR->c =temp->value.i;
            break;

            case REAL_hash:
            INSTR->c =temp->value.d;
            break;

            }
        }
            if(INSTR->specialcode==0)
            {
                if(hodnota==0)
                {
                    hodnota=INSTR->b/INSTR->c;
                    if(INSTR->c==0)
                    {
                    free_sources();          ///chyba delenie nulou
                    exit(E_DIVISION_BY_ZERO);
                    }
                hodnota3=1;   ///pomocna premenna
                }
                else
                {
                    hodnota2=INSTR->b/INSTR->c;
                    if(INSTR->c==0)
                    {
                        free_sources();
                        exit(E_DIVISION_BY_ZERO);
                    }
                hodnota3=2;
                }
            }
            else if(INSTR->specialcode==1)
            {
                if(hodnota3==1 || hodnota3==0)
                    hodnota=hodnota/INSTR->b;
                else
                    hodnota2=hodnota2/INSTR->b;
                if(INSTR->b==0)
                {
                    free_sources();
                    exit(E_DIVISION_BY_ZERO);
                }
            }
            break;

   case EQUAL:                                 ///vola sa po kazdom MOV kedy sa vycisti cela pamat
        hodnota=0;
        hodnota2=0;
        hodnota3=0;
        if (globalne_pole!=NULL)
            free(globalne_pole);
        break;

    case JUMP:                                      ///Instrukcia na skok IF skaceme len ked je false 0
        if(hodnota==0)
        {
            LABEL=INSTR->specialcode;                   ///ukladame si specialcode na zasobnik, specialne  cislo navestia
            stack_push(stackADRESS,LABEL);
            return JUMP;
        }
        else
            return NOTJUMP;

   case SAVE:                                           ///ukladame si medzivysledok vypoctu, pouziva sa hlavne pri porovnaniach
        globalne_pole1=globalne_pole;
        globalne_pole=NULL;
        hodnota4=hodnota;
        hodnota=0;
        hodnota2=0;
        hodnota3=0;
        break;

    case LESS:                                            ///Instrukcia less odcitava od seba hodnoty a pozera sa na vysledok
        if(globalne_pole!=NULL && globalne_pole1!=NULL)
        {
            g1=strlen(globalne_pole1);
            g2=strlen(globalne_pole);
            if((g1-g2)< 0)
            {
                hodnota=1;
            }
            else hodnota=0;
        }
        else if((hodnota4-hodnota)< 0)
            {
            hodnota=1;
            }
        else hodnota=0;
        break;


    case GREATER:                                                   ///Instrukcia Greater porovnava  dve hodnoty
        if(globalne_pole!=NULL && globalne_pole1!=NULL)
        {
             g1=strlen(globalne_pole1);             ///dlzka poli ak sa porovnavaju stringy
             g2=strlen(globalne_pole);
            if((g1-g2)>0)
            {
            hodnota=1;
            }
            else hodnota=0;
        }
        else  if((hodnota4-hodnota)> 0)
            {
            hodnota=1;
            }
        else hodnota=0;
        break;

    case LESSEQUAL:
        if(globalne_pole!=NULL && globalne_pole1!=NULL)
        {
            g1=strlen(globalne_pole1);
            g2=strlen(globalne_pole);
            if((g1-g2)<= 0)
            {
            hodnota=1;
            }
            else hodnota=0;
       }
       else  if((hodnota4-hodnota)<= 0)
            {
            hodnota=1;
            }
       else hodnota=0;
       break;

    case GREATEREQUAL:
        if(globalne_pole!=NULL && globalne_pole1!=NULL)
        {
            g1=strlen(globalne_pole1);
            g2=strlen(globalne_pole);
           if((g1-g2)>= 0)
            {
            hodnota=1;
            }
        else hodnota=0;
        }
        else  if((hodnota4-hodnota)>= 0)
            {
            hodnota=1;
            }
        else hodnota=0;
        break;

    case NOTEQUAL:
        if(globalne_pole!=NULL && globalne_pole1!=NULL)
        {
             g1=strlen(globalne_pole1);
             g2=strlen(globalne_pole);
            if(strcmp(globalne_pole,globalne_pole1)!=0)
                hodnota=1;
            else hodnota=0;
        }
        else if((hodnota4!=hodnota))
            {
            hodnota=1;
            }
        else hodnota=0;
        break;

    case COPYSTRINGID_:         //copy('ahoj', i, 52);                  ///ok
            globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
            if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a2);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            globalne_pole = copy(INSTR->a, temp->value.i, INSTR->c);
            break;

    case COPYSTRING_ID:         //copy('ahoj', 52, i);                 ///ok
            if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a2);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            globalne_pole = malloc(temp->value.i * sizeof(char) + 1);
            globalne_pole = copy(INSTR->a, INSTR->b, temp->value.i);
            break;

    case COPYSTRINGIDID:         //copy('ahoj', i, i);                //ok
            if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a2);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            if(INFUN!=1)
        temp2 = hashtable_search(GlobalnaTAB,INSTR->a3);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp2 = hashtable_search(RAMEC,INSTR->a3);
         if(temp2==NULL)
                temp2 = hashtable_search(GlobalnaTAB,INSTR->a3);
        }
            globalne_pole = malloc(temp2->value.i * sizeof(char) + 1);
            globalne_pole = copy(INSTR->a, temp->value.i, temp2->value.i);
            break;


    case COPYIDID_:             //copy(id,id,52);                      ///ok
            globalne_pole = malloc(INSTR->c * sizeof(char) + 1);
            if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
                 if(INFUN!=1)
        temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp2 = hashtable_search(RAMEC,INSTR->a2);
         if(temp2==NULL)
                temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            globalne_pole = copy(temp->value.str, temp2->value.i, INSTR->c);
            break;


    case COPYID_ID:             //copy(id,52,id);                     //ok
                 if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
                 if(INFUN!=1)
        temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp2 = hashtable_search(RAMEC,INSTR->a2);
         if(temp2==NULL)
                temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            globalne_pole = malloc(temp2->value.i * sizeof(char) + 1);
            globalne_pole = copy(temp->value.str, INSTR->b, temp2->value.i);
            break;


    case COPYIDIDID:             //copy(id,id,id);
        if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }


        if(INFUN!=1)
        temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp2 = hashtable_search(RAMEC,INSTR->a2);
         if(temp2==NULL)
                temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        }

        if(INFUN!=1)
        temp3 = hashtable_search(GlobalnaTAB,INSTR->a3);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp3 = hashtable_search(RAMEC,INSTR->a3);
         if(temp3==NULL)
                temp3 = hashtable_search(GlobalnaTAB,INSTR->a3);
        }
            globalne_pole = malloc(temp3->value.i * sizeof(char) + 1);
            globalne_pole = copy(temp->value.str, temp2->value.i, temp3->value.i);
            break;


    case FINDSTRSTR:
            hodnota = find(INSTR->a,INSTR->a2);
            break;

    case FINDIDSTR:
        if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }
            hodnota = find(temp->value.str, INSTR->a2);
            break;

    case FINDSTRID:
             if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a2);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a2);
        }
            hodnota = find(INSTR->a,temp->value.str);
            break;

    case FINDIDID:
        if(INFUN!=1)
        temp = hashtable_search(GlobalnaTAB,INSTR->a);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp = hashtable_search(RAMEC,INSTR->a);
         if(temp==NULL)
                temp = hashtable_search(GlobalnaTAB,INSTR->a);
        }

        if(INFUN!=1)
        temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        else
        {
         THash_table *RAMEC;
         stack_top_ramec(stackramec,&RAMEC);
         temp2 = hashtable_search(RAMEC,INSTR->a2);
         if(temp2==NULL)
                temp2 = hashtable_search(GlobalnaTAB,INSTR->a2);
        }

        hodnota = find(temp->value.str,temp2->value.str);
        break;

    case ELSE:                                          ///Else dalsi mozny skok pre IF
            LABEL=INSTR->specialcode;
            stack_push(stackADRESS,LABEL);                   ///ulozime si na zasobnik adresu
            return ELSE;
            break;

    case WHILE:
            return WHILE;

    case LOOP:
          LABEL=INSTR->specialcode;
          stack_push(stackADRESS,LABEL);
          if (hodnota==0)
          return LOOPJUMP;
          else return LOOP;
          break;

    case CALLFUN:                                       ///Instrukcia prichadza volanie funkcie  f()
        return CALLFUN;

    case VALUE:
        break;

    case DECLARE:                                             ///Vkladame nove ID do najvysieho ramca
        stack_top_ramec(stackramec,&RAMEC);
        hashtable_add(RAMEC,VARIABLE_hash,INSTR->a,(int)INSTR->b,0);
        break;

    case FMOF:                                        /// FUNNY FUNCTION FOR YOU <3  Do not try to understand
        temp=hashtable_search(GlobalnaTAB,INSTR->a);
        switch(temp->type)
        {
        case INTEGER_hash:
        hodnota=temp->value.i;
        break;

        case REAL_hash:
        hodnota=temp->value.d;
        break;

        case BOOLEAN_hash:
        hodnota=temp->value.i;
        break;

        case  STRING_hash:
        globalne_pole=temp->value.str;
        break;

        }
        break;



    }
return 0;
}

/** \brief Fukcia prechadza Instrukcnu pasku a vykonava jednotlive instrukcie
 *
 * \param ukazatel na instruknu pasku
 * \return  vsetko ok 0
 */

int searchrecord(inf_pointer_array* beh_programu)
{

int j=0;

if(((stackADRESS=stack_init())==NULL))
{
    free_sources();
    exit(E_INTERNAL);
}

if(((stackPC=stack_init())==NULL))
{
    free_sources();
    exit(E_INTERNAL);
}

if(((stackramec=stack_init_ramec())==NULL))
{
    free_sources();
    exit(E_INTERNAL);
}

if(((StackBreak=stack_init())==NULL))
{
    free_sources();
    exit(E_INTERNAL);
}


stack_push(stackADRESS,0);                                      ///inicializacia vrcholu na dolar
stack_push(stackPC,0);

hodnota2=0;
hodnota=0;
hodnota3=0;
hodnota4=0;
int RESULT;
int RESULT1=0;
int BREAKPOINT=0;

    int i=0;
    while(beh_programu->pole[i]->CODE != BEGIN_MAIN)
    {
        i++;
    }
    i++;
    while(beh_programu->pole[i]->CODE != END_MAIN)
    {

       RESULT=foo(beh_programu->pole[i]);
        switch(RESULT)
        {
/***********************************/
    case CALLFUN:
    INFUN=1;

    while(strcasecmp(beh_programu->pole[j]->a,beh_programu->pole[i]->a)!=0)
        {
           j++;

            while(beh_programu->pole[j]->a==NULL)
                j++;
        }
        struct record *IDS=hashtable_search(GlobalnaTAB,beh_programu->pole[j]->a);
        THash_table *RAMEC;
            if(((RAMEC=(THash_table*)malloc(sizeof(THash_table))) == NULL) ||
            ((RAMEC=hashtable_init(100))==0))                                        ///alokujeme hashovaciu tabulku
            exit(E_INTERNAL);


        if(IDS!=NULL && IDS->POLE_ID_LOCAL_VOLANE!=NULL)
        {
            int x=0;
            int k=0;
            int p=0;
            while (x < strlen(IDS->POLE_ID_LOCAL_VOLANE))
            {
               if(IDS->POLE_ID_LOCAL_VOLANE[x]=='$')
               {
                     i++;
                    foo(beh_programu->pole[i]);

                if(beh_programu->pole[i]->c==3)
                    {
                        temp=hashtable_search(GlobalnaTAB,beh_programu->pole[i]->a);

                        switch(temp->type)
                        {
                        case INTEGER_hash:
                         beh_programu->pole[i]->b=temp->value.i;
                        break;
                        case REAL_hash:
                           beh_programu->pole[i]->b=temp->value.d;
                        break;
                        case BOOLEAN_hash:
                            beh_programu->pole[i]->b=temp->value.i;
                            break;
                        case STRING_hash:
                            beh_programu->pole[i]->a=temp->value.str;
                            break;

                        }
                    }

                    if(beh_programu->pole[i]->c==1)                          ///BOLEAN HODNOTY
                    beh_programu->pole[i]->b=beh_programu->pole[i]->b-37;



                   char *Cislo=malloc(sizeof(char)*(x-k));
                   strncpy(Cislo,IDS->POLE_ID_LOCAL_VOLANE+k,x-k);


                   if(IDS->params[p]=='i')
                       {
                       hashtable_add(RAMEC,VARIABLE_hash,Cislo,INTEGER_hash,0);
                       struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                       HLADAJ->value.i=(int)beh_programu->pole[i]->b;
                       }
                    else if(IDS->params[p]=='s')
                       {
                       hashtable_add(RAMEC,VARIABLE_hash,Cislo,STRING_hash,0);
                       struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                       HLADAJ->value.str=malloc(sizeof(char)*strlen( beh_programu->pole[i]->a)+1);
                       strcpy(HLADAJ->value.str,beh_programu->pole[i]->a);
                       }
                    else if(IDS->params[p]=='b')
                       {
                       hashtable_add(RAMEC,VARIABLE_hash,Cislo,BOOLEAN_hash,0);
                       struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                       HLADAJ->value.i=(int)beh_programu->pole[i]->b;
                       }
                    else if(IDS->params[p]=='r')
                       {
                       hashtable_add(RAMEC,VARIABLE_hash,Cislo,REAL_hash,0);
                       struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                       HLADAJ->value.d=beh_programu->pole[i]->b;
                       }
                   k=x+1;
                   p++;
               }
              x++;
             }
        }
        stack_push(stackPC,i);
        stack_push_ramec(stackramec,RAMEC);                   ///PUSHNEME RAMEC na zasobnik
        stack_push(stackPC,j);
       i=fun(beh_programu,j);
        INFUN=0;
        j=0;
        hashtable_free(RAMEC);
    break;


    case JUMP:
        stack_top(stackADRESS,&TOP);
        if (RESULT1==0)
        RESULT1=JUMP;
        if (RESULT1==JUMP)
        RESULT1=JUMP;
        i++;
        while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
        i++;
        stack_pop(stackADRESS);
        break;



    case LOOPJUMP:
        stack_pop(StackBreak);
        stack_top(stackADRESS,&TOP);
        i++;
        while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
        i++;
        stack_pop(stackADRESS);
        break;

    case ELSE:
          stack_top(stackADRESS,&TOP);
            if (RESULT1==JUMP)
            {
                RESULT1=0;
                stack_pop(stackADRESS);
                stack_top(stackADRESS,&TOP);
                break;
            }
            else
            {
                i++;
                while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
                    i++;
            }
            stack_pop(stackADRESS);
            break;

        case WHILE:
            stack_push(StackBreak,i);
            break;
        case LOOP:
            stack_top(stackADRESS,&TOP);
                i=LOOPER(beh_programu,BREAKPOINT,i+1);
                stack_pop(stackADRESS);
                  break;

        case NOTJUMP:
            RESULT1=0;
            break;
        }
        i++;
    }
    return SUCCESS;
}

int LOOPER (inf_pointer_array* beh_programu,int BREAKPOINT,int i)
{
int RESULT;
int RESULT1=0;

    while ( beh_programu->pole[i]->CODE!=ENDJUMP || beh_programu->pole[i]->specialcode!=TOP)   ///opravit
    {

                RESULT=foo(beh_programu->pole[i]);

                    switch(RESULT)           ///SKOKY + LOOP
                    {
                    case JUMP:

                        stack_top(stackADRESS,&TOP);
                        if (RESULT1==0)
                        RESULT1=JUMP;
                        if (RESULT1==JUMP)
                        RESULT1=JUMP;
                        i++;
                        while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
                        i++;
                        stack_pop(stackADRESS);
                        stack_top(stackADRESS,&TOP);
                        break;

                   case ELSE:

                    stack_top(stackADRESS,&TOP);
                        if (RESULT1==JUMP)
                    {
                            RESULT1=0;
                            stack_pop(stackADRESS);
                            stack_top(stackADRESS,&TOP);
                            break;
                    }
                        else
                    {
                            i++;
                        while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
                            i++;
                    }
                    stack_pop(stackADRESS);
                    stack_top(stackADRESS,&TOP);
                    break;

                    case NOTJUMP:
                        RESULT1=0;
                    break;

               case WHILE:
                    stack_push(StackBreak,i);
                    break;
               case LOOP:
                    stack_top(stackADRESS,&TOP);
                        i=LOOPER(beh_programu,BREAKPOINT,i+1);
                        stack_pop(stackADRESS);
                        stack_top(stackADRESS,&TOP);
                          break;

             case LOOPJUMP:

                    stack_pop(StackBreak);
                    stack_top(StackBreak,&BREAKPOINT);
                    stack_top(stackADRESS,&TOP);
                    i++;
                    while (beh_programu->pole[i]->specialcode!=TOP || beh_programu->pole[i]->CODE!=ENDJUMP)
                    i++;
                    stack_pop(stackADRESS);
                    stack_top(stackADRESS,&TOP);
                    break;
        }


                    i++;

}

stack_top(StackBreak,&BREAKPOINT);

return BREAKPOINT;

}

int fun(inf_pointer_array* beh_programu,int j)
{
int RESULT;
int RESULT1=0;
int BREAKPOINT=0;
int n=0;
THash_table *aktRAMEC;

while(beh_programu->pole[j]->CODE!=ENDFUN)
{

RESULT=foo(beh_programu->pole[j]);


                   switch(RESULT)
                   {

                    case JUMP:
                        stack_top(stackADRESS,&TOP);
                        if (RESULT1==0)
                        RESULT1=JUMP;
                        if (RESULT1==JUMP)
                        RESULT1=JUMP;
                        j++;
                        while (beh_programu->pole[j]->specialcode!=TOP || beh_programu->pole[j]->CODE!=ENDJUMP)
                        j++;
                        stack_pop(stackADRESS);
                        stack_top(stackADRESS,&TOP);
                        break;


                    case ELSE:
                        exit(10);
                        stack_top(stackADRESS,&TOP);
                        if (RESULT1==JUMP)
                        {
                        RESULT1=0;
                        stack_pop(stackADRESS);
                        stack_top(stackADRESS,&TOP);
                        break;
                        }
                        else
                        {
                        j++;
                        while (beh_programu->pole[j]->specialcode!=TOP || beh_programu->pole[j]->CODE!=ENDJUMP)
                            j++;
                        }
                        stack_pop(stackADRESS);
                        stack_top(stackADRESS,&TOP);
                        break;
                             case WHILE:
                            stack_push(StackBreak,j);
                            break;
                        case LOOP:
                            stack_top(stackADRESS,&TOP);
                                j=LOOPER(beh_programu,BREAKPOINT,j+1);
                                stack_pop(stackADRESS);
                                  break;

                        case LOOPJUMP:

                            stack_pop(StackBreak);
                            stack_top(stackADRESS,&TOP);
                            j++;
                            while (beh_programu->pole[j]->specialcode!=TOP || beh_programu->pole[j]->CODE!=ENDJUMP)
                            j++;
                            break;


                     case CALLFUN:
                           while((strcasecmp(beh_programu->pole[n]->a,beh_programu->pole[j]->a)!=0)||(beh_programu->pole[n]->CODE!=STARTFUN))
                            {
                                n++;

                                while(beh_programu->pole[n]->a==NULL)
                                n++;
                            }
                            struct record *IDS=hashtable_search(GlobalnaTAB,beh_programu->pole[n]->a);
                            THash_table *RAMEC;
                            if(((RAMEC=(THash_table*)malloc(sizeof(THash_table))) == NULL) || ((RAMEC=hashtable_init(100))==0))                                        ///alokujeme hashovaciu tabulku
                                exit(E_INTERNAL);

                            stack_top_ramec(stackramec,&aktRAMEC);

                            if(IDS!=NULL && IDS->POLE_ID_LOCAL_VOLANE!=NULL)
                            {
                                int x=0;
                                int k=0;
                                int p=0;

                                while (x < strlen(IDS->POLE_ID_LOCAL_VOLANE))
                                {
                                   if(IDS->POLE_ID_LOCAL_VOLANE[x]=='$')
                                   {

                                        j++;
                                        foo(beh_programu->pole[j]);


                                    if(beh_programu->pole[j]->c==3)
                                        {
                                            temp=hashtable_search(aktRAMEC,beh_programu->pole[j]->a);
                                            if(temp == NULL)
                                                temp=hashtable_search(GlobalnaTAB,beh_programu->pole[j]->a);

                                            switch(temp->type)
                                            {
                                            case INTEGER_hash:
                                             beh_programu->pole[j]->b=temp->value.i;
                                            break;
                                            case REAL_hash:
                                               beh_programu->pole[j]->b=temp->value.d;
                                            break;
                                            case BOOLEAN_hash:
                                                beh_programu->pole[j]->b=temp->value.i;
                                                break;
                                            case STRING_hash:
                                                beh_programu->pole[j]->a=temp->value.str;
                                                break;

                                            }
                                        }

                                        if(beh_programu->pole[j]->c==1)                          ///BOLEAN HODNOTY
                                        beh_programu->pole[j]->b=beh_programu->pole[j]->b-37;



                                       char *Cislo=malloc(sizeof(char)*(x-k));
                                       strncpy(Cislo,IDS->POLE_ID_LOCAL_VOLANE+k,x-k);


                                       if(IDS->params[p]=='i')
                                           {
                                           hashtable_add(RAMEC,VARIABLE_hash,Cislo,INTEGER_hash,0);
                                           struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                                           HLADAJ->value.i=(int)beh_programu->pole[j]->b;
                                           }
                                        else if(IDS->params[p]=='s')
                                           {
                                           hashtable_add(RAMEC,VARIABLE_hash,Cislo,STRING_hash,0);
                                           struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                                           HLADAJ->value.str=malloc(sizeof(char)*strlen( beh_programu->pole[j]->a)+1);
                                           strcpy(HLADAJ->value.str,beh_programu->pole[j]->a);
                                           }
                                        else if(IDS->params[p]=='b')
                                           {
                                           hashtable_add(RAMEC,VARIABLE_hash,Cislo,BOOLEAN_hash,0);
                                           struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                                           HLADAJ->value.i=(int)beh_programu->pole[j]->b;
                                           }
                                        else if(IDS->params[p]=='r')
                                           {
                                           hashtable_add(RAMEC,VARIABLE_hash,Cislo,REAL_hash,0);
                                           struct record *HLADAJ=hashtable_search(RAMEC,Cislo);
                                           HLADAJ->value.d=beh_programu->pole[j]->b;
                                           }
                                       k=x+1;
                                       p++;
                                   }
                                  x++;
                                 }
                            }

                            stack_push_ramec(stackramec,RAMEC);

                            stack_push(stackPC,j);
                            stack_push(stackPC,n);
                            stack_top(stackPC,&TOP);

                            j=fun(beh_programu,n);
                            stack_top(stackPC,&TOP);
                            stack_pop(stackPC);
                            stack_top(stackPC,&TOP);
                    break;

                     case NOTJUMP:
                        RESULT1=0;
                    break;
                   }

j++;
}
stack_pop(stackPC);
stack_pop_ramec(stackramec);
stack_top(stackPC,&TOP);


return TOP;
}






