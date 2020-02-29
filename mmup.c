#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

int do_work(char *b,unsigned int len)
{
  while (len--)
    {
      *b=toupper(*b);
      b++;
    }
  return 0;
}


int do_mapped_file(const char *fn)
{
  int fd=open(fn,O_RDWR);
  struct stat finfo;
  char *b;
  if (fd==-1)
    {
      perror(fn);
      return 2;
    }
  if (fstat(fd,&finfo)==-1)
    {
      perror(fn);
      return 3;
    }
  b=mmap(NULL,finfo.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if (b==MAP_FAILED)
    {
      perror("mmap");
      return 4;
    }
  // Do work here
  //

  do_work(b,finfo.st_size);
  // Done
  msync(b,finfo.st_size,MS_ASYNC);  // not needed after Linux 2.6.19
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
