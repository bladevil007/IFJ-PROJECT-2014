
#define PODMIENKA 110

int SyntacticAnalys ();
int ERRORRET(int);
int program(int);
struct record *ELEMENT;
int progcondition();
int progfunction();
int prog();
int command(int );
int declarelist();
void value();
int fun_params();
int Libraryfunction(int);
int funkcia();
int decodederSEM(int);
int ID_ENABLE;
int Vysledok;  ///aky vysledok ma cakat pri priradeni do ID
int IN_FUNCTION; ///nachadzame sa vo funckii budeme naplnat Lokalnu tab symbolov
void free_sources();

void addparam(int);


