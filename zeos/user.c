#include <libc.h>

char buff[256] = "SOA is EASEY";
int pid;



int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  write(1, buff, strlen(buff));  
  char buff2 [150]= "\nthe getTime return value of: ";
  write(1, buff2, strlen(buff2));
  itoa(get_time(), buff);
  write(1, buff, strlen(buff));

  for(int i = 0; i < 1000000000; ++i) {}; 
    prova();

  
  while(1) { 
  }
}
