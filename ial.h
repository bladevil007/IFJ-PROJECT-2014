
#define false_hash 0
#define true_hash 1
//slouzi k urceni, zda zaznam je zaznamem funkce ci promenne
#define VARIABLE_hash 50
#define FUNCTION_hash 51
//slouzi k rozeznani datovych/navratovych typu promennych a funkci
#define INTEGER_hash 105
#define BOOLEAN_hash 103
#define REAL_hash 101
#define CHAR_hash 102
#define STRING_hash 104

typedef union //unie slouzici k ukladani hodnot jednotlivych promennych v zaznamech
{
	int i;
	double d;
	char c;
	int b;
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
	int doubledefinition;  //nabyva hodnot 0,1,2 ked mame len hlavicku je 1 ked  aj telo tak ma hodnotu 2
	char *POLE_ID_LOCAL_VOLANE;
	int valuedef;
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
int length(char *s);




/*
char *copy(char *s, int i, int n);

void failKMP(char *p, int pl, int *fail);

int matchKMP(char *search, char *txt, int pl, int tl, int *fail);

int find(char *s, char *search);

void siftDown(char *s, int left, int right);

void heapSort(char *s, int n);

char *sort(char *s);
*/
