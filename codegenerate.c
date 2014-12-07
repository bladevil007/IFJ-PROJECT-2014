/* **************************** codegenerate.c ************************************ */
/* Soubor:             codegenerate.c - Generovanie kodu a jeho vykonanie      */
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
#include <string.h>
#include "scaner.h"
#include "parser.h"
#include "string.h"
#include "err.h"
#include "ial.h"
#include "stack.h"
#include "precedent.h"
#include "codegenerate.h"

int JUMPL=5;    /// Premenna navestie skoku


/** \brief
*   Funkcia inicializuje strukturu pre nekonecne pole ukazatelov na instrukcie.
*/
int init_generate()
{
    InstructionTape=(inf_pointer_array*)malloc(sizeof(inf_pointer_array));
    init_pointer_array(InstructionTape);
    return 0;
}


    /** \brief Funkcia generuje struktury instrukcii programu a uklada ich do nekonecneho pola.
     *
     *  \param Parametre programu su komponenty struktury instrukcie:
     *
     *         Konstantne retazce:
     *          char *A
     *          char *A2
     *          char *A3
     *         Konstantne hodnoty:
     *          float B
     *          float C
     *         Identifikator instrukcie definovany v hlavickovom subore codegenerate.h:
     *          int CODE
     *         Flag so specialnym vyuzitim:
     *          int what
     */

int generate_inst(char *A,char *A2,float B,float C,int CODE,int what,char*A3)
{
    ///vytvorenie novej prazdnej struktury instrukcie ktora sa neskor naplni
    INSTape *Instruction;
    if((Instruction=(INSTape*)malloc(sizeof(INSTape)))==NULL)   ///malloc fail
    {
        free_sources();
        exit(E_INTERNAL);
    }

    ///hlavny switch zisti o aku instrukciu ide podla toho naplni prislusne hodnoty struktury,
    ///v kazdej strukture sa priradi prisluchajuci kod CODE.
    ///v instrukciach ktore maju operanty sa priraduju aj hodnoty a, a2, a3, b, c

    switch (CODE){
        case BEGIN_MAIN:    /// instrukcia zaciatok programu "main", ziadne hodnoty sa nevkladaju
            Instruction->a=NULL;
            Instruction->b=0;
            Instruction->CODE=BEGIN_MAIN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case LENGTH:        /// instrukcia vstavanej funkcie length() ktora je volana s konstantnym stringom
            Instruction->a=(char*)malloc(sizeof(char)*length(A));       ///vklada sa argument s ktorym je funkcia volana
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->CODE=LENGTH;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case LENGTHID:      /// instrukcia vstavanej funkcie length() ktora je volana s identifikatorom typu string
            Instruction->a=(char*)malloc(sizeof(char)*length(A));       ///vklada sa argument s ktorym je funkcia volana
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->CODE=LENGTHID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case END_MAIN:       /// instrukcia koniec programu "main", ziadne hodnoty sa nevkladaju
            Instruction->a=NULL;
            Instruction->b=0;
            Instruction->CODE=END_MAIN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case MOV:           /// instrukcia priradenie :=
            Instruction->a=(char*)malloc(sizeof(char)*length(A));       ///vklada sa identifikator vkladanej premennej
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->CODE=MOV;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WRITESTRING:   /// instrukcia vstavanej funkcie write() ktora je volana s konstantnym stringom
            Instruction->a=(char*)malloc(sizeof(char)*length(A));       ///vlozi sa argument s ktorym je volana
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->CODE=WRITESTRING;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WRITEINT:      /// instrukcia vstavanej funkcie write() ktora je volana s konstantnym integerom
            Instruction->a=NULL;
            Instruction->b=B;       ///vlozi sa hodnota argumentu s ktorym je volana
            Instruction->CODE=WRITEINT;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WRITEBOOL:     /// instrukcia vstavanej funkcie write() ktora je volana s bool hodnotou
            Instruction->a=NULL;
            Instruction->b=B;       ///vlozi sa hodnota argumentu s ktorym je volana 39 - true 38- false
            Instruction->CODE=WRITEBOOL;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WRITEREAL:     /// instrukcia vstavanej funkcie write() ktora je volana s realnou konstantou
            Instruction->a=NULL;
            Instruction->b=B;       ///vlozi sa hodnota argumentu s ktorym je volana
            Instruction->CODE=WRITEREAL;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WRITEID:       /// instrukcia vstavanej funkcie write() ktora je volana s lubovolnym identifikatorom
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);       /// vlozi sa nazov identifikatoru
            Instruction->b=0;
            Instruction->CODE=WRITEID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case READLN:        /// insrukcia vstavanej funkcie readln()
            Instruction->a=(char*)malloc(sizeof(char)*length(A));   /// vlozi sa identifikator s ktorym je funkcia volana
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->CODE=READLN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case SORTSTRING:    /// instrukcia vstavanej funkcie sort() volanej s konstantnym stringom
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);       ///vloz pole konstantneho stringu
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=SORTSTRING;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case SORTID:        /// instrukcia vstavanej funkcie sort() volanej s identifikatorom typu string
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);       ///vloz nazov identifikatoru
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=SORTID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case CONCATESTRING: /// instrukcia konkatenacia konstantneho retazca s globalne_pole
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);       ///vloz konstantny retazec
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=CONCATESTRING;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case CONCATEID:     /// instrukcia konkatenacia stringu zadaneho identifikatorom s globalne_pole
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);       ///vloz nazov identifikatoru
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=CONCATEID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        /// v aritmetickych instrukciach vyuzivame pomocne pole alokovane v parser.c, preto nieje potrebna alokacia tu
        case ADD:           /// instrukcia scitanie +
            Instruction->a=A;
            Instruction->a2=A2;
            Instruction->specialcode=what;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=ADD;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case MULTIPLY:      /// instrukcia nasobenie *
            Instruction->a=A;
            Instruction->a2=A2;
            Instruction->specialcode=what;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=MULTIPLY;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case DIVIDE:        /// instrukcia delenia
            Instruction->a=A;
            Instruction->a2=A2;
            Instruction->specialcode=what;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=DIVIDE;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case MINUS:         /// instrukcia delenia
             Instruction->a=A;
            Instruction->a2=A2;
            Instruction->specialcode=what;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=MINUS;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case EQUAL:         /// instrukcia porovnania
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=EQUAL;
            add_new_pointer(InstructionTape,Instruction);
            break;

        /// instrukcie vstavanej funkcie find(), pre kazdu kombinaciu argumentov (konstantny string a id) je
        /// generovana osobitna instrukcia
        /// kombinacia je zahrnuta v nazve instrukcie, do pola sa kopiruje bud nazov id alebo string
        case FINDSTRSTR:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a,A);
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=FINDSTRSTR;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case FINDIDSTR:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a,A);
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=FINDIDSTR;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case FINDSTRID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a,A);
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=FINDSTRID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case FINDIDID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a,A);
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=FINDIDID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        /// instrukcie vstavanej funkcie copy(), pre kazdu kombinaciu argumentov (konstantny string a id) je
        /// generovana osobitna instrukcia
        /// kombinacia je zahrnuta v nazve instrukcie, do pola sa kopiruje bud nazov id alebo string
        case COPYSTRING:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYSTRING;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYSTRINGID_:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYSTRINGID_;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYSTRING_ID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYSTRING_ID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYSTRINGIDID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->a3=(char*)malloc(sizeof(char)*length(A3));
            strcpy(Instruction->a3,A3);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYSTRINGIDID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYIDID_:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYIDID_;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYID_ID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYID_ID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case COPYIDIDID:
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->a2=(char*)malloc(sizeof(char)*length(A2));
            strcpy(Instruction->a2,A2);
            Instruction->a3=(char*)malloc(sizeof(char)*length(A3));
            strcpy(Instruction->a3,A3);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=COPYIDIDID;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case JUMP:      /// instrukcia skoku vklada do komponenty b index instrukcie na ktoru sa skace a inkrementuje
                        /// flag JUMPL aby sme vedeli kam sa vratit
            JUMPL++;
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->specialcode=JUMPL;
            Instruction->c=C;
            Instruction->CODE=JUMP;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case BEGINJUMP: /// instrukcia skoku na zaciatok programu
            JUMPL++;
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->specialcode=JUMPL;
            Instruction->c=C;
            Instruction->CODE=0;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case ENDJUMP:   /// instrukcia skoku na koniec programu
            Instruction->a=NULL;
            Instruction->specialcode=JUMPL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=ENDJUMP;
            add_new_pointer(InstructionTape,Instruction);
            JUMPL--;
            break;

        case SAVE:      /// instrukcia ukladania lavej strany podmienky ako pomoc pri vyhodnocovani
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=SAVE;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case LESS:      /// instrukcia binarna operacia mensitko
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=LESS;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case ELSE:      /// instrukcia skok na vetvu else
            JUMPL++;
            Instruction->a=NULL;
            Instruction->specialcode=JUMPL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=ELSE;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case WHILE:     /// instrukcia cyklu while
            Instruction->a=NULL;
            Instruction->specialcode=0;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=WHILE;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case LOOP:      /// instrukcia vykonanie cyklu
            JUMPL++;
            Instruction->a=NULL;
            Instruction->specialcode=JUMPL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=LOOP;
            add_new_pointer(InstructionTape,Instruction);
            break;

        /// nasleduju instrukcie porovnavania
        case GREATER:
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=GREATER;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case LESSEQUAL:
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=LESSEQUAL;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case GREATEREQUAL:
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=GREATEREQUAL;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case NOTEQUAL:
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=NOTEQUAL;
            add_new_pointer(InstructionTape,Instruction);
            break;

       case STARTFUN:       /// instrukcia zaciatku funkcie
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);   ///nazov funkcie
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=STARTFUN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case ENDFUN:        /// instrukcia koniec funkcie
            Instruction->a=NULL;
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=ENDFUN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case CALLFUN:       /// vyvolanie funkcie
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->b=0;
            Instruction->c=0;
            Instruction->CODE=CALLFUN;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case VALUE:
            if(A!=NULL)
            {
                Instruction->a=(char*)malloc(sizeof(char)*length(A));
                strcpy(Instruction->a,A);
            }
            Instruction->b=B;
            Instruction->c=what;
            Instruction->CODE=VALUE;
            add_new_pointer(InstructionTape,Instruction);
            break;

        case DECLARE:       /// deklaruj
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=DECLARE;
            add_new_pointer(InstructionTape,Instruction);
        break;

        case FMOF:          /// instrukcia priradenie navratovej hodnoty funkcie do id
            Instruction->a=(char*)malloc(sizeof(char)*length(A));
            strcpy(Instruction->a,A);
            Instruction->b=B;
            Instruction->c=C;
            Instruction->CODE=FMOF;
            add_new_pointer(InstructionTape,Instruction);
        break;
    }

    return 0;
}

/** \brief  Funkcia inicializuje komponenty struktury nekonecneho pola ukazatelov ukazatelov na instrukcie.
 *          Prvotna alokacia pola je na 100 prvkov.
 *  \param  Ukazatel na strukturu inf_pointer_array.
 */

void init_pointer_array(inf_pointer_array *a)
{
    a->pole = (INSTape **)malloc(sizeof(INSTape)*100);
    if(a->pole == NULL)
        {
            free_sources();
            exit(E_INTERNAL);
        }
    a->allocSize = 100;
    a->length = 0;

    return;
}


/** \brief  Funkcia postupne uvolni alokovanu pamat vnutri struktur instrukcii a nakoniec uvolni samotne pole
 *          ukazatelov.
 *  \param  Ukazatel na strukturu inf_pointer_array.
 */

void free_pointer_array(inf_pointer_array *a)
{
    for(int i=0; i < a->length; i++)
    {
        if(a->pole[i]->a != NULL)
            free(a->pole[i]->a);

        if(a->pole[i]->a2 != NULL)
            free(a->pole[i]->a2);

        if(a->pole[i]->a3 != NULL)
            free(a->pole[i]->a);
    }

    free(a->pole);
    return;
}


/** \brief  Funkcia prida novy ukazatel na instrukciu do nekonecneho pola instrukcii.
 *
 *  \param  Ukazatel na strukturu inf_pointer_array - nekonecne pole.
 *  \param  Ukazatel na pridavanu instrukciu.
 */

int add_new_pointer(inf_pointer_array *a, INSTape* pointer)
{
    if((a->allocSize - a->length) <= 2)    ///ak je nedostatok volneho miesta v poli, tak realokuj
    {
        a->pole = realloc(a->pole, a->allocSize + (100 * sizeof(INSTape*)) );
        if (a->pole == NULL)               ///realloc fail
            {
                free_sources();
                exit(E_INTERNAL);
            }
        a->allocSize = a->allocSize + 100; ///aktualizacia premennej
    }

    a->pole[a->length] = pointer;          ///vloz novy ukazatel na aktualny index
    a->length = a->length + 1;             ///aktualizuj index v poli

    return 1;
}
