#ifndef HASH
#define HASH

#define false 0
#define true 1
//slouzi k urceni, zda zaznam je zaznamem funkce ci promenne
#define VARIABLE 50
#define FUNCTION 51
//slouzi k rozeznani datovych/navratovych typu promennych a funkci
#define INTEGER 100
#define REAL 101
#define CHAR 102
#define STRING 104

typedef union //unie slouzici k ukladani hodnot jednotlivych promennych v zaznamech
{
	int i;
	double d;
	char c;
	char *str;
} DRtype; 

struct record 
{ //struktura pro zaznam v hash tabulce
	int id; //nabyva hodnot FUNCTION a VARIABLE - urcuje, zda je identifikator promenna ci funkce
	char *key; //ukazatel na identifikator(nazev) funkce/promenne
	int type; //u funkce je tato promenna nositelem informace o navratovem typu, u promenne informace o datovem typu
	DRtype value; //u promennych slouzi k ukladani jejich hodnot
	char *params; //u funkce slouzi k ulozeni informace o typech jejich parametru, u promennych ma hodnotu NULL
	int defined; //nabyva hodnot true a false - u promenne urcuje, zda je v promenne prirazena nejaka hodnota, u funkce urcuje, zda jiz bylo definovano jeji telo
	struct record *next;	
};

typedef struct 
{ //struktura pro hash-tabulku
	unsigned hashtable_size;
	struct record *ptr[];
} THash_table;

//prototypy funkci:
void stringtolower (char *key);
THash_table *hashtable_init (unsigned size);
unsigned int hash_function (const char *str, unsigned hashtable_size);
void hashtable_clear (THash_table *table);
void hashtable_free(THash_table *table);
struct record *hashtable_search(THash_table *table, char *key);
struct record *hashtable_add(THash_table *table, int id, char *key, int type, char *params);
#endif
