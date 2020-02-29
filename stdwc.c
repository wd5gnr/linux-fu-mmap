#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int wc(int cin)
{
  static int c=0;  // characters
  static int l=0;    // lines
  static int w=0;    // words
  static int state=0;  // 0 means no word in progress, 1 means word in progress
  if (cin==-1) // end of file so dump results and reset
    {
      printf("%d\t%d\t%d\n",l,w,c);
      l=0;
      w=0;
      state=0;
      c=0;
    }
  else  // process a character
    {
      c++;
      if (cin=='\n') l++;
      if (isspace(cin))
	{
	if (state==1)
	  {
	    w++;
	    state=0;
	  }
	}
      else state=1;
    }

  return 0;
}


int do_file(const char *fn)
{
  FILE *f=fopen(fn,"r");
  if (!f)
    {
      perror(fn);
      return 2;
    }
  while (!feof(f))
    {
      wc(getc(f));
    }
  fclose(f);
  return 0;
}
  




int main(int argc, char *argv[])
{
  if (argc<2)
    {
      fprintf(stderr,"Usage: mmwc file\n");
      return 1;
    }
  while (--argc)
    {
      char *fn=*++argv;  // get next file name
      int rv;
      if ((rv=do_file(fn))!=0) return rv ;
    }
  return 0;
}
