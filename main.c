#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
FILE *source;

/*source file opening*/

  if (argc == 1)
   {
      fprintf(stderr,"No input file\n");
      return 0;
   }

   if ((source = fopen(argv[1],"r")) == NULL)
   {
      fprintf(stderr,"Failed to open\n");

      return 0;
   }












close(source);
return 0;
}
