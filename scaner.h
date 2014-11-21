
// TOKENY
#include "string.h"
void SourceInitialize(FILE *f);
int getnextToken (LEX_STRUCT *LEX_STRUCTPTR);

#define ID  10            // IDENTIFICATOR
#define BEGIN 11
#define BOOLEAN 12
#define DO 13
#define ELSE 14
#define END 15
#define FIND 17
#define FORWARD 18
#define FUNCTION 19
#define IF 20
#define INTEGER 21
#define READLN 22          // LOAD FROM STDIN
#define REAL 23            //
#define SORT 24            //
#define STRING 25          //
#define THEN 26            // THEN
#define VAR 28             // DECLARE GLOBAL VARIABLE
#define WHILE 29           // WHILE
#define WRITE 30           // PRINTF
#define PLUS 31            // +
#define MINUS 32           // -
#define MULTIPLY 33        // *
#define DIVIDE 34          // /
#define DOT 35             // .
#define EXPONENT 36        //e , E
#define APOSTROF 37        //ASCII 39 + string
#define CHAR 38            //#i (256)
#define TRUE 39            // BOOLEAN VALUE
#define FALSE 38           // BOOLEAN VALUE
#define KOMENT 39
#define EOFILE 40
#define DVOJBODKA 41       //:
#define BODKOCIARKA 42     //;
#define CIARKA 43          //,
#define GREATER 44         //>
#define LESS 45            //<
#define EQUAL 46           // =
#define LENGTH 47          // FUNCTION
#define COPY 48            // FUNCTION
#define CONST 49           // NUMBER
#define LEFT_HRANATA 60
#define RIGHT_HRANATA 61
#define SUCCESS 52
#define FAIL 53
#define LEFT_ROUND 54                 //ZATVORKA
#define RIGHT_ROUND 55
#define CONST_STRING 56
#define REALo 57
#define HASHTAG 58
#define LESSEQUAL 59
#define GREATEREQUAL 60
#define NOTEQUAL 61


