#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int do_work(const char *b,unsigned int len)
{
  int c=len;  // characters
  int l=0;    // lines
  int w=0;    // words
  int state=0;  // 0 means no word in progress, 1 means word in progress
  while (len--)  // process each character
    {
      if (*b=='\n') l++;
      if (isspace(*b))
	{
	if (state==1)
	  {
	    w++;
	    state=0;
	  }
	}
      else state=1;
      b++;
    }
  printf("%d\t%d\t%d\n",l,w,c);  // dump result
  return 0;
}


int do_mapped_file(const char *fn)
{
  int fd=open(fn,O_RDONLY);   // open file
  struct stat finfo;
  char *b;
  if (fd==-1)
    {
      perror(fn);
      return 2;
    }
  if (fstat(fd,&finfo)==-1)   // learn size of file
    {
      perror(fn);
      return 3;
    }
  b=mmap(NULL,finfo.st_size,PROT_READ,MAP_PRIVATE,fd,0);  // map to memory
  if (b==MAP_FAILED)
    {
      perror("mmap");
      return 4;
    }
  // Do work here
  do_work(b,finfo.st_size);
  // Done
  munmap(b,finfo.st_size);
  close(fd);
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
      if ((rv=do_mapped_file(fn))!=0) return rv ;
    }
  return 0;
}
